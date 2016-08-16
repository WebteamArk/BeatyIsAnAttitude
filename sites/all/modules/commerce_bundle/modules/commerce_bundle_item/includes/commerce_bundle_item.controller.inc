<?php

/**
 * @file
 * The controller for the bundle item entity containing the CRUD operations.
 */

/**
 * The controller class for bundle item contains methods for the entity CRUD operations.
 *
 * Mainly relies on the EntityAPIController class provided by the Entity
 * module, just overrides specific features.
 */
class CommerceBundleItemEntityController extends EntityAPIController {

  /**
   * Create a default bundle item.
   *
   * @param array $values
   *   An array of values to set, keyed by property name.
   * @return
   *   An object with all default fields initialized.
   */
  public function create(array $values = array()) {
    $values += array(
      'item_id' => NULL,
      'type' => '',
      'status' => 1,
      'created' => '',
      'changed' => '',
    );

    return parent::create($values);
  }

  /**
   * Saves a bundle item.
   *
   * @param $entity
   *   The full entity object to save.
   * @param $transaction
   *   An optional transaction object.
   *
   * @return
   *   SAVED_NEW or SAVED_UPDATED depending on the operation performed.
   */
  public function save($entity, DatabaseTransaction $transaction = NULL) {
    global $user;

    // Hardcode the changed time.
    $entity->changed = REQUEST_TIME;

    if (empty($entity->{$this->idKey}) || !empty($entity->is_new)) {
      // Set the creation timestamp if not set, for new entities.
      if (empty($entity->created)) {
        $entity->created = REQUEST_TIME;
      }
    }
    else {
      // Otherwise if the entity is not new but comes from an entity_create()
      // or similar function call that initializes the created timestamp and uid
      // value to empty strings, unset them to prevent destroying existing data
      // in those properties on update.
      if ($entity->created === '') {
        unset($entity->created);
      }
    }

    // Determine if we will be inserting a new entiy.
    $entity->is_new = empty($entity->{$this->idKey});

    return parent::save($entity, $transaction);
  }

  /**
   * Deletes multiple entities by ID.
   *
   * @param $$entity_ids
   *   An array of $entity IDs to delete.
   * @param $transaction
   *   An optional transaction object.
   *
   * @return
   *   TRUE on success, FALSE otherwise.
   */
  public function delete($entity_ids, DatabaseTransaction $transaction = NULL) {
    if (!empty($entity_ids)) {
      $entities = $this->load($entity_ids, array());

      // Ensure the bundle items can actually be deleted.
      foreach ((array) $entities as $entity_id => $entity) {
        if (!commerce_bundle_item_can_delete($entity)) {
          unset($entities[$entity_id]);
        }
      }

      // If none of the specified entities can be deleted, return FALSE.
      if (empty($entities)) {
        return FALSE;
      }

      parent::delete(array_keys($entities), $transaction);
      return TRUE;
    }
    else {
      return FALSE;
    }
  }

  /**
   * Builds a structured array representing the entity's content.
   *
   * The content built for the entity will vary depending on the $view_mode
   * parameter.
   *
   * @param $entity
   *   An entity object.
   * @param $view_mode
   *   View mode, e.g. 'full', 'teaser'...
   * @param $langcode
   *   (optional) A language code to use for rendering. Defaults to the global
   *   content language of the current request.
   * @return
   *   The renderable array.
   */
  public function buildContent($entity, $view_mode = 'full', $langcode = NULL, $content = array()) {
    // Add the default fields inherent to the entity.
    $content['status'] = array(
      '#markup' => theme('commerce_bundle_item_status', array('status' => $entity->status, 'label' => t('Status:'), 'entity' => $entity)),
    );

    return parent::buildContent($entity, $view_mode, $langcode, $content);
  }
}
