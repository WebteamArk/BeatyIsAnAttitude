<?php

/**
 * @file
 * Forms for creating / editing and deleting bundle items.
 */


/**
 * Form callback: create or edit a bundle item.
 *
 * @param $entity
 *   The entity object to edit through the form.
 */
function commerce_bundle_item_form($form, &$form_state, $entity, $op, $entity_type) {
  // Ensure this include file is loaded when the form is rebuilt from the cache.
  $form_state['build_info']['files']['form'] = drupal_get_path('module', 'commerce_bundle_item') . '/includes/commerce_bundle_item.forms.inc';

  // Add the field related form elements.
  $langcode = entity_language('commerce_bundle_item', $entity);
  if (empty($langcode)) {
    $langcode = LANGUAGE_NONE;
  }
  
  field_attach_form('commerce_bundle_item', $entity, $form, $form_state, $langcode);

  // Additional entity properties
  $form['status'] = array(
    '#type' => 'radios',
    '#title' => t('Status'),
    '#description' => t('Disabled bundle items cannot be added to shopping carts.'),
    '#options' => array(
      '1' => t('Active'),
      '0' => t('Disabled'),
    ),
    '#default_value' => $entity->status,
    '#required' => TRUE,
    '#weight' => 50,
  );

  // We add the form's #submit array to this button along with the actual submit
  // handler to preserve any submit handlers added by a form callback_wrapper.
  $submit = array();

  if (!empty($form['#submit'])) {
    $submit += $form['#submit'];
  }

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save', array(), array('context' => 'a drupal commerce bundle item')),
    '#submit' => array_merge($submit, array('commerce_bundle_item_form_submit')),
    '#weight' => 40,
  );

  $item = menu_get_item('admin/commerce/bundle-items');
  if (!empty($item) && $item['access']) {
    $form['actions']['submit']['#suffix'] = l(t('Cancel'), 'admin/commerce/bundle-items');
  }
  
  // We append the validate handler to #validate in case a form callback_wrapper
  // is used to add validate handlers earlier.
  $form['#validate'][] = 'commerce_bundle_item_form_validate';

  return $form;
}

/**
 * Validation callback for commerce_bundle_item_form().
 */
function commerce_bundle_item_form_validate($form, &$form_state) {
  $entity = $form_state[$form_state['entity_type']];

  // Notify field widgets to validate their data.
  field_attach_form_validate('commerce_bundle_item', $entity, $form, $form_state);
}

/**
 * Submit callback for commerce_bundle_item_form().
 */
function commerce_bundle_item_form_submit($form, &$form_state) {
  global $user;

  $entity = $form_state[$form_state['entity_type']];

  // If the user is editing, load a fresh copy to merge changes to.
  if (!empty($entity->item_id)) {
    $entity = commerce_bundle_item_load($entity->item_id);
  }

  // Merge changes into the entity object in the form state so it is accessible
  // by field handlers.
  $form_state[$form_state['entity_type']] = $entity;

  // Notify field widgets.
  field_attach_submit('commerce_bundle_item', $entity, $form, $form_state);

  // Save the entity.
  $entity->save();

  drupal_set_message(t('Item saved.'));
}
