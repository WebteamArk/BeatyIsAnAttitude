<?php

/**
 * Export Drupal Commerce bundle items to Views.
 */

/**
 * Implements hook_views_data()
 */
function commerce_bundle_item_views_data() {
  $data = array();

  $data['commerce_bundle_item']['table']['group']  = t('Commerce Bundle Item');

  $data['commerce_bundle_item']['table']['base'] = array(
    'field' => 'item_id',
    'title' => t('Commerce Bundle Items'),
    'help' => t('Bundle items from the store.'),
    'access query tag' => 'commerce_bundle_item_access',
  );
  $data['commerce_bundle_item']['table']['entity type'] = 'commerce_bundle_item';


  // Expose the item ID.
  $data['commerce_bundle_item']['item_id'] = array(
    'title' => t('Item ID'),
    'help' => t('The unique internal identifier of the bundle item.'),
    'field' => array(
      'handler' => 'commerce_bundle_item_handler_field_item',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );

  // Expose the bundle item type.
  $data['commerce_bundle_item']['type'] = array(
    'title' => t('Type'),
    'help' => t('The human-readable name of the type of the bundle item.'),
    'field' => array(
      'handler' => 'commerce_bundle_item_handler_field_item_type',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'commerce_bundle_item_handler_filter_item_type',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // Expose the bundle item status.
  $data['commerce_bundle_item']['status'] = array(
    'title' => t('Status'),
    'help' => t('Whether or not the bundle item is active.'),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
      'output formats' => array(
        'active-disabled' => array(t('Active'), t('Disabled')),
      ),
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
      'label' => t('Active'),
      'type' => 'yes-no',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // Expose the created and changed timestamps.
  $data['commerce_bundle_item']['created'] = array(
    'title' => t('Created date'),
    'help' => t('The date the bundle item was created.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );

  $data['commerce_bundle_item']['created_fulldate'] = array(
    'title' => t('Created date'),
    'help' => t('In the form of CCYYMMDD.'),
    'argument' => array(
      'field' => 'created',
      'handler' => 'views_handler_argument_node_created_fulldate',
    ),
  );

  $data['commerce_bundle_item']['created_year_month'] = array(
    'title' => t('Created year + month'),
    'help' => t('In the form of YYYYMM.'),
    'argument' => array(
      'field' => 'created',
      'handler' => 'views_handler_argument_node_created_year_month',
    ),
  );

  $data['commerce_bundle_item']['created_timestamp_year'] = array(
    'title' => t('Created year'),
    'help' => t('In the form of YYYY.'),
    'argument' => array(
      'field' => 'created',
      'handler' => 'views_handler_argument_node_created_year',
    ),
  );

  $data['commerce_bundle_item']['created_month'] = array(
    'title' => t('Created month'),
    'help' => t('In the form of MM (01 - 12).'),
    'argument' => array(
      'field' => 'created',
      'handler' => 'views_handler_argument_node_created_month',
    ),
  );

  $data['commerce_bundle_item']['created_day'] = array(
    'title' => t('Created day'),
    'help' => t('In the form of DD (01 - 31).'),
    'argument' => array(
      'field' => 'created',
      'handler' => 'views_handler_argument_node_created_day',
    ),
  );

  $data['commerce_bundle_item']['created_week'] = array(
    'title' => t('Created week'),
    'help' => t('In the form of WW (01 - 53).'),
    'argument' => array(
      'field' => 'created',
      'handler' => 'views_handler_argument_node_created_week',
    ),
  );

  $data['commerce_bundle_item']['changed'] = array(
    'title' => t('Updated date'),
    'help' => t('The date the bundle item was last updated.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );

  $data['commerce_bundle_item']['changed_fulldate'] = array(
    'title' => t('Updated date'),
    'help' => t('In the form of CCYYMMDD.'),
    'argument' => array(
      'field' => 'changed',
      'handler' => 'views_handler_argument_node_created_fulldate',
    ),
  );

  $data['commerce_bundle_item']['changed_year_month'] = array(
    'title' => t('Updated year + month'),
    'help' => t('In the form of YYYYMM.'),
    'argument' => array(
      'field' => 'changed',
      'handler' => 'views_handler_argument_node_created_year_month',
    ),
  );

  $data['commerce_bundle_item']['changed_timestamp_year'] = array(
    'title' => t('Updated year'),
    'help' => t('In the form of YYYY.'),
    'argument' => array(
      'field' => 'changed',
      'handler' => 'views_handler_argument_node_created_year',
    ),
  );

  $data['commerce_bundle_item']['changed_month'] = array(
    'title' => t('Updated month'),
    'help' => t('In the form of MM (01 - 12).'),
    'argument' => array(
      'field' => 'changed',
      'handler' => 'views_handler_argument_node_created_month',
    ),
  );

  $data['commerce_bundle_item']['changed_day'] = array(
    'title' => t('Updated day'),
    'help' => t('In the form of DD (01 - 31).'),
    'argument' => array(
      'field' => 'changed',
      'handler' => 'views_handler_argument_node_created_day',
    ),
  );

  $data['commerce_bundle_item']['changed_week'] = array(
    'title' => t('Updated week'),
    'help' => t('In the form of WW (01 - 53).'),
    'argument' => array(
      'field' => 'changed',
      'handler' => 'views_handler_argument_node_created_week',
    ),
  );

  // Expose links to operate on the bundle item.
  $data['commerce_bundle_item']['view_item'] = array(
    'field' => array(
      'title' => t('Link'),
      'help' => t('Provide a simple link to the administrator view of the bundle item.'),
      'handler' => 'commerce_bundle_item_handler_field_item_link',
    ),
  );
  $data['commerce_bundle_item']['edit_item'] = array(
    'field' => array(
      'title' => t('Edit link'),
      'help' => t('Provide a simple link to edit the bundle item.'),
      'handler' => 'commerce_bundle_item_handler_field_item_link_edit',
    ),
  );
  $data['commerce_bundle_item']['delete_item'] = array(
    'field' => array(
      'title' => t('Delete link'),
      'help' => t('Provide a simple link to delete the bundle item.'),
      'handler' => 'commerce_bundle_item_handler_field_item_link_delete',
    ),
  );

  $data['commerce_bundle_item']['operations'] = array(
    'field' => array(
      'title' => t('Operations links'),
      'help' => t('Display all the available operations links for the bundle item.'),
      'handler' => 'commerce_bundle_item_handler_field_item_operations',
    ),
  );


  return $data;
}
