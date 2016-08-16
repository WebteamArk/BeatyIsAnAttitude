<?php

/**
 * @file
 * Provides the migration class for commerce_bundle_item entities.
 */

/**
 * Migration class for commerce_bundle_item entities.
 */
class CommerceMultichannelMigration_commerce_bundle_item extends CommerceMultichannelMigrationBase {

  /**
   * {@inheritdoc}
   */
  public function __construct(array $arguments = array()) {
    parent::__construct($arguments + array(
      'entity_type' => 'commerce_bundle_item',
      'group_name' => 'commerce_multichannel_products',
    ));
  }
}
