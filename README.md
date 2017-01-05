# upitt_islandora_findingaids_relationships

This module allows you to manage the relationships for finding aids.  The relationship between an object and the finding aid is that the object would have the isMemberOf pointing to the finding aid in question.  There is no relationship info stored on the finding aids' RELS-EXT.

This module displays the possible objects on the /manage page that come from the EAD file's ns2 xlink href "barcodes" (which in our system equate to the object identifier by adding the "pitt:" prefix).  The actual namespace prefix that is appended to each ns2 xlink barcode is configurable in the admin/islandora/tools/findingaidrelationship form.  Relationships can be added / removed one at a time, or all relationships can be created by clicking the "Add ALL object relationships for this finding aid".  See below.  ![manage-finding-aid-relationships](https://cloud.githubusercontent.com/assets/19391126/21398288/d1147684-c774-11e6-827d-98605c0e9729.png)

Relationships between finding aids and any manuscriptCModel/bookCModel/sp_basic_image/sp_large_image_cmodel objects can also be reviewed / changed by this module when viewing the object's /manage page. See below. ![manage-manuscript-relationships](https://cloud.githubusercontent.com/assets/19391126/21398748/8d5a1c8a-c776-11e6-8154-835d66b229e2.png)

It may also be desirable to display the relationship to the finding aids at an object level.  Once the isMemberOf relationship exists on the objects, it is a simple matter of displaying the RELS_EXT_isMemberOf_uri_ms values in the metadata.


If the "add all objects to finding aid" functionality needs to be used in an other module's script code, simply include the findingaids.form.inc - and call like this: 

```
  // Given $findingaid_pid is any PID for a finding aid that exists in the system.
  module_load_include('inc', 'upitt_islandora_findingaids_relationships', 'includes/findingaidsobjects.form');
  _do_add_all_findingaids_relationships($findingaid_pid);
```

**NOTE:**  Relationships made by this module are not hooked into changes made to the EAD, so hypothetically if an EAD xml is uploaded to replace a former copy that had more barcodes in it, there may be unintentional dangling relationships to those objects.  If it is known that the barcodes are different in a new copy of an EAD, it is highly recommended that all of the relationships be removed **BEFORE** replacing an EAD.
