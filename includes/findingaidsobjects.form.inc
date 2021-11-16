<?php

/**
 * @file
 * Handles the isMemberOf relationships to finding aids for objects.
 */

/**
 * The add isMemberOf finding aids form.
 */
function upitt_islandora_findingaids_relationships_add($form, &$form_state, AbstractObject $object, $object_id = '') {
  $findingaid_pid = ($object_id == '') ? '' : urldecode($object_id);
  $child_id = $object->id;

  if ($findingaid_pid == 'all') {
    // actually have to switch the variables in this special context.
    $findingaid_pid = $child_id;
    $child_id = 'all';
    $form = array(
      'detail' => array(
        '#type' => 'fieldset',
        '#title' => 'Clicking save will relate the finding aid with all objects found in the EAD file.'
      ),
    );
  }

  $form['finding_aid'] = array(
      '#type' => 'textfield',
      '#title' => t('Finding Aid PID'),
      '#description' => t('Provide the Finding Aid to which this object is related.  Auto-complete will search using the title of the Finding Aid.'),
      '#size' => 45,
      '#autocomplete_path' => 'islandora_findingaids_relationships/findingaidsobjects',
      '#default_value' => $findingaid_pid,
    );
  $form['child_id'] = array(
      '#type' => 'value',
      '#value' => $child_id,
    );
  $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save'),
    );

  if ($child_id == 'all') {
    $form['finding_aid']['#type'] = 'hidden';
  }
  return $form;
}

/**
 * Submit handler for upitt_islandora_findingaids_relationships_add.
 */
function upitt_islandora_findingaids_relationships_add_submit($form, &$form_state) {
  $form_state['redirect'] = '/islandora/object/' . $form_state['values']['child_id'] . '/manage';
  if ($form_state['values']['child_id'] == 'all') {
    $findingaid_pid = $form_state['values']['finding_aid'];
    _do_add_all_findingaids_relationships($findingaid_pid);
    drupal_goto('/islandora/object/' . $findingaid_pid . '/manage', array());
  }
  else {
    $object = islandora_object_load($form_state['values']['child_id']);
    if (_do_add_relationship($form_state['values']['finding_aid'], $object)) {
      drupal_set_message(t('The "isMemberOf" relationship has been added to "@child_id".', array('@child_id' => $form_state['values']['child_id'])));
    }
  }
}

/**
 * The remove memberof handler.
 */
function upitt_islandora_findingaids_relationships_remove($form, &$form_state, AbstractObject $object, $findingaid_pid) {
  $form = array();
  $child_object = $object->id;
  
  $form = array(
    'child_object' => array(
      '#type' => 'value',
      '#value' => $child_object,
    ),
    'child_id' => array(
      '#type' => 'value',
      '#value' => urldecode($findingaid_pid),
    ),
  );

  return confirm_form(
    $form,
    t('Are you sure you want to delete the "isMemberOf" relationship between "@object" and object "@child_object"?', array('@child_object' => $child_object, '@object' => urldecode($findingaid_pid))),
    "/islandora/object/{$object->id}/manage/",
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Submit handler for upitt_islandora_findingaids_relationships_remove.
 */
function upitt_islandora_findingaids_relationships_remove_submit($form, &$form_state) {
  $form_state['redirect'] = '/islandora/object/' . $form_state['values']['child_object'] . '/manage';
  $child_object = islandora_object_load($form_state['values']['child_object']);    
  if (is_object($child_object) && isset($form_state['values']['child_id'])) {
    // Remove the relationship that matches the string value for $child_object.
    if ($child_object->relationships->remove(FEDORA_RELS_EXT_URI, 'isMemberOf', $form_state['values']['child_id'])) {
      drupal_set_message(t('The "isMemberOf" relationship has been removed for "@object".', array('@object' => $child_object->id)));
    }
    else {
      drupal_set_message(t('There was a problem attempting to remove the "isMemberOf" relationship for "@object".', array('@object' => $child_object->id)));
    }
  }
}

/**
 * Function that will attempt to add the "isMemberOf" relationship for all objects associated to a finding aid EAD.  This is called
 * from the "add" form (just below) -- and allows a call from a script to this function.  This will also convert the related book objects to manuscripts.
 *
 * @param string $findingaid_pid
 *   The PID value for the finding aid object.
 */
function _do_add_all_findingaids_relationships($findingaid_pid) {
  $object = islandora_object_load($findingaid_pid);
  $referenced_objects = findingaid_EAD_objects($object);
  $added_ids = $skipped_ids = array();
  $related_objects = findingaid_objects($object->id);

  foreach ($referenced_objects as $object_pid => $child_rec_array) {
    $object_link = l($object_pid, "islandora/object/{$object_pid}");

    $related = FALSE;
    $exists = FALSE;
    if (!array_key_exists($object_pid, $related_objects)) {
      $rels = $child_rec_array['RELS_EXT_isMemberOf_uri_ms'];
      $fedora_pid = 'info:fedora/' . $findingaid_pid;
      foreach ($rels as $rel) {
        $related |= ($rel == $fedora_pid);
      }
      // if the related object is a book, make it into a manuscript object
      if (isset($child_rec_array['RELS_EXT_hasModel_uri_ms']) && _is_book($child_rec_array['RELS_EXT_hasModel_uri_ms'])) {
        $child_object = islandora_object_load($object_pid);
        if (is_object($child_object)) {
          $child_object->models = _manuscript_model_not_book($child_rec_array['RELS_EXT_hasModel_uri_ms']);
        }
      }

      if (!$related) {
        if (_do_add_relationship($findingaid_pid, $object_pid)) {
          $added_ids[] = $object_pid;
        }
      }
      else {
        $skipped_ids[] = $object_pid;
      }
    }
  }
  if (count($added_ids) > 0) {
    drupal_set_message(t('The "isMemberOf" relationship has been added between "@finding_aid" and "@object".', array('@finding_aid' => $findingaid_pid, '@objects' => implode(", ", $added_ids))));
  }
  if (count($skipped_ids) > 0) {
    drupal_set_message(t('Adding a "isMemberOf" relationship to "@finding_aid" was skipped because it already existed.  Objects: @objects', array('@finding_aid' => $findingaid_pid, '@objects' => implode(", ", $skipped_ids))));
  }
}

/**
 * Helper function to add the relationship between a finding aid an an object.
 * 
 * @param string $findingaid_pid
 *   The PID value for the finding aid object.
 *
 * @param object OR string $child
 *   When passed a string, need to load the object, else uses the object that is passed.
 *
 */
function _do_add_relationship($findingaid_pid, $child) {
  $child_object = (is_object($child)) ? $child : islandora_object_load($child);
  if (is_object($child_object)) {
    $child_object->relationships->add(FEDORA_RELS_EXT_URI, 'isMemberOf', $findingaid_pid);
  }
}

/**
 * Helper function to determine whether or not the object is a bookCModel
 */
function _is_book($models) {
  $retval = FALSE;
  if (is_array($models)) {
    $retval = (!(array_search('islandora:bookCModel', $models) === FALSE));
  }
  return $retval;
}

/**
 * Helper function to return the models - after replacing the book instance with a manuscript reference.
 */
function _manuscript_model_not_book($models = array()) {
  $updated_models = array();
  foreach ($models as $model) {
    if ($model <> 'islandora:bookCModel') {
      $updated_models[] = $model;
    }
    else {
      $updated_models[] = 'islandora:manuscriptCModel';
    }
  }
  return $updated_models;
}
