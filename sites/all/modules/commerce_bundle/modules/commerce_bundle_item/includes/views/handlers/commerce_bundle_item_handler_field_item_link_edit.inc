<?php

/**
 * Field handler to present a bundle item edit link.
 */
class commerce_bundle_item_handler_field_item_link_edit extends commerce_bundle_item_handler_field_item_link {
  function construct() {
    parent::construct();

    $this->additional_fields['type'] = 'type';
  }

  function render($values) {
    // Ensure the user has access to edit this item.
    $entity_type = 'commerce_bundle_item';
    $entity = entity_create($entity_type, array(
      'item_id' => $this->get_value($values, 'item_id'),
      'type' => $this->get_value($values, 'type'),
    ));
    
    if (!entity_access('update', $entity_type, $entity)) {
      return;
    }

    $text = !empty($this->options['text']) ? $this->options['text'] : t('edit');

    return l($text, 'admin/commerce/bundle-items/' . $entity->item_id . '/edit', array('query' => drupal_get_destination()));
  }
}
