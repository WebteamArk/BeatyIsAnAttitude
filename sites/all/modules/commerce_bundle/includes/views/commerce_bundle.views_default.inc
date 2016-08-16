<?php

/**
 * Implements hook_views_default_views_alter().
 */
function commerce_bundle_views_default_views_alter(&$views) {
  if (isset($views['commerce_cart_form'])) {
    $handler =& $views['commerce_cart_form']->display['default']->handler;
    // First remove the cart forms default line item id sort.
    unset($handler->display->display_options['sorts']['line_item_id']);
    // Add a sort handler based on bundle_config_id.
    /* Sort criterion: Commerce Line item: Bundle Configuration ID (commerce_bundle_config_id) */
    $handler->display->display_options['sorts']['commerce_bundle_config_id_value']['id'] = 'commerce_bundle_config_id_value';
    $handler->display->display_options['sorts']['commerce_bundle_config_id_value']['table'] = 'field_data_commerce_bundle_config_id';
    $handler->display->display_options['sorts']['commerce_bundle_config_id_value']['field'] = 'commerce_bundle_config_id_value';
    $handler->display->display_options['sorts']['commerce_bundle_config_id_value']['relationship'] = 'commerce_line_items_line_item_id';
  }
}
