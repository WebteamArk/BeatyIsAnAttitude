<?php

/**
 * @file
 */


/**
 * Menu callback: display an overview of available bundle item types.
 */
function commerce_bundle_item_types_overview() {
  drupal_add_css(drupal_get_path('module', 'commerce_bundle_item') . '/theme/commerce_bundle_item.admin.css');

  $header = array(
    t('Name'),
    t('Operations'),
  );

  $rows = array();

  // Loop through all defined bundle item types.
  foreach (commerce_bundle_item_types() as $type => $item_type) {
    // Build the operation links for the current item type.
    $links = menu_contextual_links('commerce-bundle-item-type', 'admin/commerce/config/bundle-items', array(commerce_bundle_item_type_to_arg($type)));

    // Add the item type's row to the table's rows array.
    $type_markup = check_plain($item_type['name']);
    $type_markup .= ' <small>' . t('(Machine name: @type)', array('@type' => $item_type['type'])) . '</small>';
    $type_markup .= '<div class="description">' . filter_xss_admin($item_type['description']) . '</div>';

    $rows[] = array(
      $type_markup,
      theme('links', array('links' => $links, 'attributes' => array('class' => 'links inline operations'))),
    );
  }

  // If no item types are defined...
  if (empty($rows)) {
    // Add a standard empty row with a link to add a new item type.
    $rows[] = array(
      array(
        'data' => t('There are no bundle item types.'),
        'colspan' => 2,
      )
    );
  }

  return theme('table', array('header' => $header, 'rows' => $rows));
}
