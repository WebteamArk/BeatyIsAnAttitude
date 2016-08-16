<?php

/**
 * @file
 * Field handler to present a button to remove a line item or a bundle of line
 * items
 */


class commerce_bundle_handler_field_edit_delete extends commerce_line_item_handler_field_edit_delete {

  /**
   * Returns the form which replaces the placeholder from render().
   */
  function views_form(&$form, &$form_state) {
    // The view is empty, abort.
    if (empty($this->view->result)) {
      return;
    }

    $form[$this->options['id']] = array(
      '#tree' => TRUE,
    );

    // At this point, the query has already been run, so we can access the results
    // in order to get the base key value (for example, nid for nodes).
    foreach ($this->view->result as $row_id => $row) {

      // Extract field data containing entity information.
      $alias = $this->aliases['line_item_id'];
      $data =& $row->_field_data[$alias];
      $form_data =& $form[$this->options['id']][$row_id];
      if (isset($data['entity']->type) && $data['entity']->type == 'commerce_bundle_line_item') {
        // Only the control row of a bundle gets a 'Remove' button.
        $form_data['#line_item_id'] = $data['entity']->line_item_id;
        continue;
      }

      $form[$this->options['id']][$row_id] = array(
        '#type' => 'submit',
        '#name' => 'delete-line-item-' . $row_id,
        '#attributes' => array('class' => array('delete-line-item')),
        '#submit' => array_merge($form['#submit'], array('commerce_line_item_line_item_views_delete_form_submit')),
      );

      if ($data['entity_type'] == 'bundle_control') {
        $form_data['#value'] = t('Remove Bundle');
        continue;
      }

      $line_item_id = $this->get_value($row);
      $form_data['#value'] = t('Delete');
      $form_data['#line_item_id'] = $line_item_id;
    }
  }

  function views_form_submit($form, &$form_state) {
    $order = commerce_order_load($form_state['order']->order_id);
    $field_name = $this->options['id'];

    $alias = $this->aliases['line_item_id'];

    foreach (element_children($form[$field_name]) as $row_id) {
      // Check for the removal of an item.
      if ($form_state['triggering_element']['#name'] == 'delete-line-item-' . $row_id) {
        $row_data = $this->view->result[$row_id]->_field_data[$alias];
        if ($row_data['entity_type'] == 'bundle_control') {
          // It's a bundle control row. Grab all the row indices that are part
          // of this bundle.
          foreach ($this->view->result as $row_index => $data) {
            if ($data->_field_data[$alias]['entity_type'] != 'bundle_control') {
              $line_item_wrapper = entity_metadata_wrapper('commerce_line_item', $data->_field_data[$alias]['entity']);
              if (isset($line_item_wrapper->commerce_bundle_id) && $row_data['entity']->commerce_bundle_config_id == $line_item_wrapper->commerce_bundle_config_id->value()) {
                $bundle_row_indices[] = $row_index;
              }
            }
          }

          // Process deletion of all the line items in the bundle.
          foreach ($bundle_row_indices as $index) {
            $line_item_id = $form[$field_name][$index]['#line_item_id'];
            commerce_cart_order_product_line_item_delete($order, $line_item_id);
          }
          continue;
        }

        $line_item_id = $form[$field_name][$row_id]['#line_item_id'];
        // TODO: Remove this dependence on the Cart module API.
        commerce_cart_order_product_line_item_delete($order, $line_item_id);
      }
    }
  }
}
