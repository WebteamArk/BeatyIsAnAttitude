<?php

/**
 * Field handler to present a bundle item's operations links.
 */
class commerce_bundle_item_handler_field_item_operations extends views_handler_field {
  function construct() {
    parent::construct();

    $this->additional_fields['item_id'] = 'item_id';
  }

  function option_definition() {
    $options = parent::option_definition();

    $options['add_destination'] = TRUE;

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['add_destination'] = array(
      '#type' => 'checkbox',
      '#title' => t('Add a destination parameter to operations links so users return to this View on form submission.'),
      '#default_value' => $this->options['add_destination'],
    );
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {
    $item_id = $this->get_value($values, 'item_id');

    // Get the operations links.
    $links = menu_contextual_links('commerce-bundle-item', 'admin/commerce/bundle-items', array($item_id));

    if (!empty($links)) {
      // Add the destination to the links if specified.
      if ($this->options['add_destination']) {
        foreach ($links as $id => &$link) {
          $link['query'] = drupal_get_destination();
        }
      }

      drupal_add_css(drupal_get_path('module', 'commerce_bundle_item') . '/theme/commerce_bundle_item.admin.css');
      return theme('links', array('links' => $links, 'attributes' => array('class' => array('links', 'inline', 'operations'))));
    }
  }
}
