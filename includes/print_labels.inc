<?php

/**
 * @file
 * Handles the isMemberOf relationships to finding aids for objects.
 */

/**
 * Parse the EAD for the given object and create the spreadsheet for download.
 *
 * @param AbstractObject $object
 */
function upitt_islandora_findingaids_relationships_print_labels(AbstractObject $object) {
  // Inspect the current $object to be sure it is a finding aid that has
  // an EAD datastream.
  if (is_object($object) && !(array_search('islandora:findingAidCModel', $object->models) === FALSE) && isset($object['EAD'])) {
    $csv_file = upitt_islandora_findingaids_relationships_make_print_labels($object['EAD']->content);
    if ($csv_file) {
      $filename = 'print_labels_' . str_replace(":", "_", $object->id) . '.csv';
      header('Content-type:text/plain; charset=UTF-8');
      header("Content-Disposition: attachment; filename=\"$filename\"");
      print $csv_file;
    }
    else {
      drupal_set_message('There was a problem creating the download file for the finding aid.  There was no content created from the EAD\'s containers.');
      drupal_goto($_SERVER['HTTP_REFERER']);
    }
  }
  else {
    drupal_set_message('There was a problem creating the download file for the finding aid.  Either the object is not a finding aid object or it does not have an EAD datastream.');
    drupal_goto($_SERVER['HTTP_REFERER']);
  }
}

function upitt_islandora_findingaids_relationships_make_print_labels($ead_content) {
  // Run the transform on it to get the array of containers needed to create
  // each row of the spreadsheet.
  if ($ead_content) {
    // use a transform to update the value?
    $containers = upitt_islandora_findingaids_relationships_runXslTransform(array(
        'xsl' => drupal_get_path('module', 'upitt_islandora_findingaids_relationships') . '/transforms/recurse.xsl',
        'input' => $ead_content,
    ));
    // The containers should be tab delimited -- should convert to CSV
    $csv_list = array();
    $lines = explode("\n", $containers);
    foreach ($lines as $line) {
      $csv_list[] = explode("\t", $line);
    }

    $temp_filename = drupal_realpath(drupal_tempnam("temporary://", "print_labels_csv"));
    $fp = fopen($temp_filename, 'w');
    foreach ($csv_list as $fields) {
      fputcsv($fp, $fields);
    }
    @fclose($fp);

    $csv_file = file_get_contents($temp_filename);
    unlink($temp_filename);
    
    return $csv_file;
  }
}

// COPIED directly from islandora_batch/includes/islandora_scan_batch.inc.
/**
  * Run an XSLT, and return the results.
  *
  * @param array $info
  *   An associative array of parameters, containing:
  *   - input: The input XML in a string.
  *   - xsl: The path to an XSLT file.
  *   - php_functions: Either a string containing one or an array containing
  *     any number of functions to register with the XSLT processor.
  *
  * @return string
  *   The transformed XML, as a string.
  */
function upitt_islandora_findingaids_relationships_runXslTransform($info) {
  $xsl = new DOMDocument();
  $xsl->load($info['xsl']);
  
  $input = new DOMDocument();
  $input->loadXML($info['input']);

  $processor = new XSLTProcessor();
  $processor->importStylesheet($xsl);

  if (isset($info['php_functions'])) {
    $processor->registerPHPFunctions($info['php_functions']);
  }

  return $processor->transformToXML($input);
}
