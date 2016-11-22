<?php

/**
 * @file
 * Administration forms and submit handling for isMemberOfSite module.
 */

/**
 * Administration and configuration form for memberofsite.
 *
 * @ingroup forms
 *
 * @todo break out non-settings elements into their own form.
 *
 * @return array
 *   The Drupal form definition.
 */
function upitt_islandora_findingaids_relationships_admin_form() {
  $form = array(
    'namespace' => array(
      '#type' => 'textfield',
      '#title' => t('Namespace'),
      '#description' => t('Value to use for xmlns attributes on IsMemberOfSite associations.'),
      '#default_value' => variable_get('upitt_islandora_findingaids_relationships_namespace', 'http://digital.library.pitt.edu/ontology/relations#'),
      '#size' => 100,
    ),
    'sites' => array(
      '#type' => 'textarea',
      '#title' => t('Sites'),
      '#description' => t('Value to use for the rdf:resource attribute on IsMemberOfSite associations.  List possible site values (must match the "!link" value from those sites\' configurations.)',
        array('!link' => l(t('Solr base filter'), '/admin/islandora/search/islandora_solr/settings'))),
      '#default_value' => variable_get('upitt_islandora_findingaids_relationships_sites'),
    ),
    'submit' => array(
      '#type' => 'submit',
      '#value' => t('Save configuration'),
    ),
    'reset' => array(
      '#type' => 'submit',
      '#value' => t('Reset to defaults'),
    ),
  );
  return $form;
}

/**
 * Function that sets the Drupal variables with user's input.
 *
 * @param array $form
 *   The Drupal form definition.
 * @param array $form_state
 *   The Drupal form state.
 */
function upitt_islandora_findingaids_relationships_admin_form_submit(array $form, array &$form_state) {
  drupal_set_message(t('The settings have been updated!'));
  $id = $form_state['triggering_element']['#id'];
  switch ($id) {
    case 'edit-submit':
      variable_set('upitt_islandora_findingaids_relationships_namespace', $form_state['values']['namespace']);
      variable_set('upitt_islandora_findingaids_relationships_sites', $form_state['values']['sites']);
      break;

    case 'edit-reset':
      variable_del('upitt_islandora_findingaids_relationships_namespace');
      variable_del('upitt_islandora_findingaids_relationships_sites');
      break;
  }
}

/**
 * Admin autocomplete callback which returns all possible Finding Aids objects via a Solr query.
 *
 * @param string $string
 *   String filled out in the autocomplete textfield.
 *
 * @return json
 *   A json array containing the finding aids values that contain the given string.
 */
function _islandora_findingaids_autocomplete($string = '') {
  if ($string) {
    $solr_query = format_string('!field:!value AND RELS_EXT_hasModel_uri_ms:islandora\:findingAidCModel', array(
                  '!field' => 'fgs_label_s',
                  '!value' => '*' . $string . '*'
        ));
  }
  else {
    $solr_query = 'RELS_EXT_hasModel_uri_ms:islandora\:findingAidCModel');
  }
  $options = array('wt' => 'json',
                   'fl' => 'PID,fgs_label_s');
  $solr_docs = islandora_solr($solr_query, $options);

/// 
//   $luke = islandora_solr_get_luke();
  $result = array();
  foreach ($solr_docs as $solr_doc_obj) {
    $term = $solr_doc_obj['fgs_label_s'];
    if (stripos($term, $string) !== FALSE) {
      // Search case insensitive, but keep the case on replace.
      $term_str = preg_replace("/$string/i", "<strong>\$0</strong>", $term);

      // Add strong elements to highlight the found string.
      $result[$solr_doc_obj['PID']] = $term_str;
    }
  }
  // Sort alphabetically.
  ksort($result);

  drupal_json_output($result);
  exit();
}
