<?php

/**
 * Filter by bundle item type.
 */
class commerce_bundle_item_handler_filter_item_type extends views_handler_filter_in_operator {
  // Display a list of item types in the filter's options.
  function get_value_options() {
    if (!isset($this->value_options)) {
      $this->value_title = t('Bundle item type');
      $this->value_options = commerce_bundle_item_type_options_list();
    }
  }
}
