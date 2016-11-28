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
    'namespace_prefix' => array(
      '#type' => 'textfield',
      '#title' => t('Namespace'),
      '#description' => t('Value to use for generating islandora object identifiers for EAD barcode values (include the colon).  <em>Example: "pitt:"</em>'),
      '#default_value' => variable_get('upitt_islandora_findingaids_relationships_namespace_prefix', 'pitt:'),
      '#size' => 100,
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
      variable_set('upitt_islandora_findingaids_relationships_namespace_prefix', $form_state['values']['namespace_prefix']);
      break;

    case 'edit-reset':
      variable_del('upitt_islandora_findingaids_relationships_namespace_prefix');
      break;
  }
}
