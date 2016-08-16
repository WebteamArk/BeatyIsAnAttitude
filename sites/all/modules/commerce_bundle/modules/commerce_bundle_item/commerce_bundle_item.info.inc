<?php

/**
 * @file
 * Provides metadata for the bundle item entity.
 */

/**
 * Implements hook_entity_property_info().
 */
function commerce_bundle_item_entity_property_info() {
  $info = array();

  // Add meta-data about the basic properties.
  $properties = &$info['commerce_bundle_item']['properties'];

  $properties['item_id'] = array(
    'label' => t('Item ID'),
    'description' => t('The internal numeric ID of the bundle item.'),
    'type' => 'integer',
    'schema field' => 'item_id',
  );
  $properties['type'] = array(
    'label' => t('Type'),
    'description' => t('The type of the bundle item.'),
    'type' => 'token',
    'setter callback' => 'entity_property_verbatim_set',
    'setter permission' => 'administer commerce_bundle_item entities',
    'options list' => 'commerce_bundle_item_type_options_list',
    'required' => TRUE,
    'schema field' => 'type',
  );
  $properties['status'] = array(
    'label' => t('Status'),
    'description' => t('Boolean indicating whether the bundle item is active or disabled.'),
    'type' => 'boolean',
    'options list' => 'commerce_bundle_item_status_options_list',
    'setter callback' => 'entity_property_verbatim_set',
    'setter permission' => 'administer commerce_bundle_item entities',
    'schema field' => 'status',
  );
  $properties['created'] = array(
    'label' => t('Date created'),
    'description' => t('The date the bundle item was created.'),
    'type' => 'date',
    'setter callback' => 'entity_property_verbatim_set',
    'setter permission' => 'administer commerce_bundle_item entities',
    'schema field' => 'created',
  );
  $properties['changed'] = array(
    'label' => t('Date updated'),
    'description' => t('The date the bundle item was most recently updated.'),
    'type' => 'date',
    'setter callback' => 'entity_property_verbatim_set',
    'query callback' => 'entity_metadata_table_query',
    'setter permission' => 'administer commerce_bundle_item entities',
    'schema field' => 'changed',
  );

  return $info;
}
