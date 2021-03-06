<?php

/**
 * @file
 * Contains the basic bundle item field handler.
 */

/**
 * Field handler to provide simple renderer that allows linking to a bundle item.
 */
class commerce_bundle_item_handler_field_item extends views_handler_field {
  function init(&$view, &$options) {
    parent::init($view, $options);

    if (!empty($this->options['link_to_entity'])) {
      $this->additional_fields['item_id'] = 'item_id';
    }
  }

  function option_definition() {
    $options = parent::option_definition();

    $options['link_to_entity'] = array('default' => FALSE);

    return $options;
  }

  /**
   * Provide the link to entiy option.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['link_to_entity'] = array(
      '#title' => t("Link this field to the bundle items's administrative view page"),
      '#description' => t('This will override any other link you have set.'),
      '#type' => 'checkbox',
      '#default_value' => !empty($this->options['link_to_entity']),
    );
  }

  /**
   * Render whatever the data is as a link to the entity.
   *
   * Data should be made XSS safe prior to calling this function.
   */
  function render_link($data, $values) {
    if (!empty($this->options['link_to_entity']) && $data !== NULL && $data !== '') {
      $item_id = $this->get_value($values, 'item_id');
      $this->options['alter']['make_link'] = TRUE;
      $this->options['alter']['path'] = 'admin/commerce/bundle-items/' . $item_id;
    }

    return $data;
  }

  function render($values) {
    $value = $this->get_value($values);
    return $this->render_link($this->sanitize_value($value), $values);
  }
}
