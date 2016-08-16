<?php

/**
 * Provide bundles integration with shopping cart and related Views.
 */

/**
 * Implements hook_views_data_alter().
 */
function commerce_bundle_views_data_alter(&$data) {
  // Override the line item title handler
  $data['commerce_line_item']['line_item_title']['field']['handler'] = 'commerce_bundle_handler_field_line_item_title';
  $data['commerce_line_item']['edit_quantity']['field']['handler'] = 'commerce_bundle_handler_field_edit_quantity';
  $data['commerce_line_item']['edit_delete']['field']['handler'] = 'commerce_bundle_handler_field_edit_delete';
}

/**
 * Implements hook_views_post_execute().
 */
function commerce_bundle_views_post_execute(&$view) {
  if ($view->name == 'commerce_cart_form') {

    // First we need to get the alias for line item id.
    foreach ($view->query->field_aliases as $field_array) {
      if (array_key_exists('line_item_id', $field_array)) {
        $alias = $field_array['line_item_id'];
        break;
      }
    }

    if (is_array($view->result) && !empty($view->result)) {
      $control_rows = array();
      $current_config_id = NULL;
      // Inspect each row to determine where bundle control row headers need
      // to be placed, and add bundle css classes.
      foreach ($view->result as $row_index => $current_row) {
        $current_entity = $current_row->_field_data[$alias]['entity'];
        // We don't care about anything but bundle line items.
        if ($current_entity->type != 'commerce_bundle_line_item') {
          continue;
        }
        // Now we're dealing with a bundle line item.
        $current_entity_wrapper = entity_metadata_wrapper('commerce_line_item', $current_entity);

        $previous_row = isset($view->result[$row_index - 1]) ? $view->result[$row_index - 1] : NULL;
        $previous_entity_wrapper = isset($previous_row) ? entity_metadata_wrapper('commerce_line_item', $previous_row->_field_data[$alias]['entity']) : NULL;

        // Store where the appropriate css classes need to go.
        if (isset($current_config_id)) {
          if ($current_config_id == $current_entity_wrapper->commerce_bundle_config_id->value()) {
            // Sets the current row's bundle_row_classes.
            $view->result[$row_index]->bundle_row_classes = array('bundle-item', 'bundle-config-'. $current_entity_wrapper->commerce_bundle_config_id->value());
            // The row gets a last class if either it's the last in the view
            // results array, or the next row doesn't have a config_id or
            // the next row has a config_id but it's different.
            $next_row = isset($view->result[$row_index + 1]) ? $view->result[$row_index + 1] : NULL;
            $next_entity_wrapper = isset($next_row) ? entity_metadata_wrapper('commerce_line_item', $next_row->_field_data[$alias]['entity']) : NULL;
            if (!isset($next_row) || $next_row->_field_data[$alias]['entity']->type != 'commerce_bundle_line_item' || $next_entity_wrapper->commerce_bundle_config_id->value() != $current_entity_wrapper->commerce_bundle_config_id->value()) {
              $view->result[$row_index]->bundle_row_classes[] = 'bundle-last';
            }
          }
        }

        if (!isset($previous_row) || $previous_row->_field_data[$alias]['entity']->type != 'commerce_bundle_line_item' || $previous_entity_wrapper->commerce_bundle_config_id->value() != $current_entity_wrapper->commerce_bundle_config_id->value()) {
          // It's a first bundle line item in the group.
          // We make sure to add css classes for the current row.
          $view->result[$row_index]->bundle_row_classes = array('bundle-item', 'bundle-config-'. $current_entity_wrapper->commerce_bundle_config_id->value());
          // We create a control row and note the row index that it needs to be
          // spliced into.
          $new_row = new stdClass();
          $new_row->_field_data[$alias]['entity_type'] = 'bundle_control';
          $new_row->_field_data[$alias]['entity'] = new stdClass();
          // Set the display path to the current row's.
          $path = 'node/' . $current_entity_wrapper->commerce_bundle_id->nid->value();
          // Set the display path for the control row.
          $new_row->field_commerce_display_path = $current_row->field_commerce_display_path;
          $new_row->field_commerce_display_path[0]['rendered']['#markup'] = $path;
          $new_row->field_commerce_display_path[0]['raw']['value'] = $path;
          $new_row->field_commerce_display_path[0]['raw']['safe_value'] = $path;

          // Add some dummy markup to remove Views render warnings.
          $new_row->field_commerce_unit_price[0]['rendered']['#markup'] = '';
          $new_row->field_commerce_total[0]['rendered']['#markup'] = '';

          $data =& $new_row->_field_data[$alias]['entity'];
          $data->commerce_bundle_id = $current_entity_wrapper->commerce_bundle_id->value();
          $data->commerce_product = $current_entity_wrapper->commerce_product->value();
          $data->order_id = $current_entity->order_id;
          $data->commerce_bundle_config_id = $current_entity_wrapper->commerce_bundle_config_id->value();

          // We set the latest config id so that we know which rows need bundle
          // classes.
          $current_config_id = $data->commerce_bundle_config_id;

          // We store the insertion point (ie row index) and the control row
          // data to build the new view result after inspection.
          $control_rows[] = array (
            'row_index' => $row_index,
            'row_data' => $new_row,
          );
        }
      }

      // Add the control rows after parsing all line items in the cart.
      foreach ($control_rows as $row_count => $control_row) {
        // Insertion point for the view is going to equal the row index plus the
        // number of control rows already inserted.
        $insertion_point = $control_row['row_index'] + $row_count;
        array_splice($view->result, $insertion_point, 0, array($control_row['row_data']));
        // We add css classes to the control row that are appropriate.
        $view->result[$insertion_point]->bundle_row_classes = array('bundle-title', 'bundle-config-'. $control_row['row_data']->_field_data[$alias]['entity']->commerce_bundle_config_id);
      }
    }
  }
}

/**
 * Adds the classes to the rows that were tagged in hook_views_post_execute().
 * @param array $vars
 *   Raw data needed for processing a view.
 */
function commerce_bundle_preprocess_views_view_table(&$vars) {
  // We only care about the Shopping cart form.
  if ($vars['view']->name != 'commerce_cart_form') {
    return;
  }
  foreach ($vars['view']->result as $row_index => $row) {
    if (isset($row->bundle_row_classes)) {
      $vars['row_classes'][$row_index] = array_merge($vars['row_classes'][$row_index], $row->bundle_row_classes);
    }
  }
  $path = drupal_get_path('module', 'commerce_bundle');
  drupal_add_css($path . '/theme/commerce_bundle_cart.css');
}
