<?php

/**
 * @file
 * Provides default fields for bundle items.
 */


/**
 * Fields and instances managed by this module
 *
 * @param $type
 *   The specific bundle item type to build the definitions
 *
 * @return
 *   An array keyed with 'fields' and 'instances' managed by this module
 */
function _commerce_bundle_item_type_default_fields($type = NULL) {
  $entity_type = 'commerce_bundle_item';

  // Fields
  $fields = array(
    'commerce_bundle_product' => array(
      'type' => 'entityreference',
      'cardinality' => 1,
      'translatable' => FALSE,
      'locked' => TRUE,
      'settings' => array(
        'target_type' => 'commerce_product',
        'handler' => 'base',
        'handler_settings' => array(),
      ),
    ),
    'commerce_bundle_price' => array(
      'type' => 'commerce_price',
      'cardinality' => 1,
      'translatable' => FALSE,
      'locked' => TRUE,
    ),
  );

  // Field instances.
  $instances = array(
    'commerce_bundle_product' => array(
      'label' => t('Product'),
      'required' => TRUE,
      'settings' => array(),
      'widget' => array(
        'type' => 'entityreference_autocomplete',
        'module' => 'entityreference',
        'weight' => -5,
      ),
      'display' => array(
        'default' => array(
          'label' => 'hidden',
          'type' => 'entityreference_label',
          'weight' => -5,
          'settings' => array(
            'link' => TRUE,
          ),
        ),
      ),
    ),
    'commerce_bundle_price' => array(
      'label' => t('Price'),
      'required' => FALSE,
      'settings' => array(),

      // Because this widget is locked, we need it to use the full price widget
      // since the currency option can't be adjusted at the moment.
      'widget' => array(
        'type' => 'commerce_price_full',
        'weight' => -4,
        'settings' => array(
          'currency_code' => 'default',
        ),
      ),

      'display' => array(
        'default' => array(
          'label' => 'hidden',
          'type' => 'commerce_price_formatted_amount',
          'weight' => -4,
          'settings' => array(
            'calculation' => FALSE,
          ),
        ),
      ),
    ),
  );

  $info = array('fields' => $fields, 'instances' => $instances);
  $context = array(
    'entity_type' => $entity_type,
    'bundle' => $type,
  );

  _commerce_bundle_item_merge_field_info_defaults($info, $context);
  return $info;
}


// -----------------------------------------------------------------------
// Helpers

/**
 * Set common field and instance definition properties
 *
 * @param $info
 *   An array of 'fields' and 'instance' defintions
 * @param $context
 *   An array of 'entity_type' and 'bundle'
 */
function _commerce_bundle_item_merge_field_info_defaults(&$info, $context) {
  $entity_type = !empty($context['entity_type']) ? $context['entity_type'] : NULL;
  $entity_type_exists = !empty($entity_type);

  $bundle = !empty($context['bundle']) ? $context['bundle'] : NULL;
  $bundle_exsits = !empty($bundle);

  if (!empty($info['fields'])) {
    // Set entity type
    foreach ($info['fields'] as $field_name => &$field) {
      $field['field_name'] = $field_name;
      if ($entity_type_exists) {
        $field['entity_types'] = array($entity_type);
      }
    }
    unset($field);
  }

  if (!empty($info['instances'])) {
    // Set entity type and bundle

    foreach ($info['instances'] as $field_name => &$instance) {
      $instance['field_name'] = $field_name;
      $instance['entity_type'] = $entity_type;

      if ($bundle_exsits) {
        $instance['bundle'] = $bundle;
      }
    }
    unset($instance);
  }
}
