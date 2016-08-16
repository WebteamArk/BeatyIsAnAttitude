<?php

/**
 * @file
 * Provides a UI controller for bundle items.
 */

/**
 * UI controller providing UI for bundle item entities.
 */
class CommerceBundleItemUIController extends EntityBundleableUIController {

  /**
   * Provides definitions for implementing hook_menu().
   */
  public function hook_menu() {
    $items = parent::hook_menu();
    $wildcard = isset($this->entityInfo['admin ui']['menu wildcard']) ? $this->entityInfo['admin ui']['menu wildcard'] : '%entity_object';

    // Unset the manage entity paths, as the provided UI is for admin entities.
    unset($items[$this->path], $items[$this->path . '/list']);

    $weight = -1;
    foreach (array_keys($items) as $item_path) {
      // Unset all manage/* paths
      if (strpos($item_path, $this->path . '/manage/') === 0) {
        unset($items[$item_path]);
      }
      else {
        // Set weight
        $items[$item_path]['weight'] = $weight++;
      }
    }

    // Set title for add
    $items[$this->path . '/add']['title'] = 'Add an item';
    unset($items[$this->path . '/add']['title callback'],  $items[$this->path . '/add']['title arguments']);
    return $items;
  }

  /**
   * Operation form submit callback.
   */
  public function operationFormSubmit($form, &$form_state) {
    parent::operationFormSubmit($form, $form_state);

    if (empty($form_state['redirect']) || $form_state['redirect'] == '<front>') {
      $item = menu_get_item('admin/commerce/bundle-items');
      if (!empty($item) && $item['access']) {
        $form_state['redirect'] = 'admin/commerce/bundle-items';
      }
    }
  }
}

