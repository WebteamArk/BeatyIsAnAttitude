<?php

/**
 * Field handler for bundle line item title integration.
 */
class commerce_bundle_handler_field_line_item_title extends commerce_line_item_handler_field_line_item_title {

  function render($values) {
    // Extract field data containing entity information.
    $alias = $this->aliases['line_item_id'];
    $data =& $values->_field_data[$alias];
    if ($data['entity_type'] == 'bundle_control') {
      // It's a control row. We return the node display title as the row title.
      return $data['entity']->commerce_bundle_id->title;
    }
    else {
      return parent::render($values);
    }
  }
}
