# upitt_islandora_memberofsite


To use the "add all objects to finding aid" function, simply include the findingaids.form.inc - and call like this: 

 module_load_include('inc', 'upitt_islandora_findingaids_relationships', 'includes/findingaidsobjects.form');
 _do_add_all_findingaids_relationships('islandora:24');
