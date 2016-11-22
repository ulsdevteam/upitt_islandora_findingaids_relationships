<?php

/**
 * @file
 * Handles the isMemberOf relationships to finding aids for objects.
 */

/**
 * The add isMemberOf finding aids form.
 */
function upitt_islandora_findingaids_relationships_add($form, &$form_state, AbstractObject $object) {
  $findingaid = variable_get('upitt_islandora_findingaids_relationships_findingaid');

  $obj_findingaid = $object->relationships->get(NULL, 'isMemberOf');
  $existing_findingaid = $newfindingaid = array();
  if (count($obj_findingaid) > 0) {
    foreach ($obj_findingaid as $obj) {
      if (isset($obj['object']['value'])) {
        $existing_findingaid[] = $obj['object']['value'];
      }
    }
  }
  $findingaid_arr = explode("\r\n", $findingaid);
  foreach ($findingaid_arr as $element) {
    if (($element) && (array_search($element, $existing_findingaid) === FALSE)) {
      $newfindingaid[$element] = $element;
    }
  }
  $form = array(
    'finding_aid' => array(
      '#type' => 'textfield',
      '#title' => t('Finding Aid'),
      '#description' => t('Finding Aid to which this object is related.'),
      '#size' => 45,
      '#autocomplete_path' => 'islandora_findingaids_relationships/findingaidsobjects',
      '#default_value' => '',
    ),
    'object_id' => array(
      '#type' => 'value',
      '#value' => $object->id,
    ),
    'submit' => array(
      '#type' => 'submit',
      '#value' => t('Save'),
    ),
  );
  return $form;
}

/**
 * Submit handler for upitt_islandora_findingaids_relationships_add.
 */
function upitt_islandora_findingaids_relationships_add_submit($form, &$form_state) {
  $object = islandora_object_load($form_state['values']['object_id']);
  $object->relationships->add(variable_get('upitt_islandora_findingaids_relationships_namespace'), 'isMemberOf', $form_state['values']['finding_aid']);
  drupal_set_message(t('The "isMemberOf" relationship has been added to "@object_id".', array('object_id' => $form_state['values']['object_id'])));
  $form_state['redirect'] = "islandora/object/{$form_state['values']['object_id']}/manage/";
}

/**
 * The remove memberof handler.
 */
function upitt_islandora_findingaids_relationships_remove($form, &$form_state, AbstractObject $object, $ns, $findingaid) {
  $form = array();
  $ns = urldecode($ns);
  $findingaid = urldecode($findingaid);
  $form = array(
    'ns' => array(
      '#type' => 'value',
      '#value' => $ns,
    ),
    'findingaid' => array(
      '#type' => 'value',
      '#value' => $findingaid,
    ),
    'object_id' => array(
      '#type' => 'value',
      '#value' => $object->id,
    ),
  );

  return confirm_form(
    $form,
    t('Are you sure you want to delete the "isMemberOf" relationship from "@findingaid" in the "@ns" resource?', array('findingaid' => $findingaid, 'ns' => $ns)),
    "islandora/object/{$object->id}/manage/",
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Submit handler for upitt_islandora_findingaids_relationships_remove.
 */
function upitt_islandora_findingaids_relationships_remove_submit($form, &$form_state) {
  $ns = $form_state['values']['ns'];
  $findingaid = $form_state['values']['finding_aid'];
  if (isset($form_state['values']['ns']) && isset($form_state['values']['object_id'])) {
    $object = islandora_object_load($form_state['values']['object_id']);
    // Remove the relationship that matches the string value for $ns / $findingaid.
    if ($object->relationships->remove($ns, 'isMemberOf', $form_state['values']['finding_aid'])) {
      drupal_set_message(t('The "isMemberOf" relationship has been removed for "@findingaid" in the "@ns" resource.', array('findingaid' => $findingaid, 'ns' => $ns)));
    }
    else {
      drupal_set_message(t('There was a problem attempting to remove the "isMemberOf" relationship for "@findingaid" in the "@ns" resource.', array('findingaid' => $findingaid, 'ns' => $ns)));
    }
  }
  $form_state['redirect'] = "islandora/object/{$object->id}/manage/";
}
