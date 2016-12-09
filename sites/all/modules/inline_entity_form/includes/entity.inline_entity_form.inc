<?php

/**
 * @file
 * Defines the base inline entity form controller.
 */

class EntityInlineEntityFormController {

  protected $entityType;
  public $settings;

  public function __construct($entityType, array $settings) {
    $this->entityType = $entityType;
    $this->settings = $settings + $this->defaultSettings();
  }

  /**
   * Returns an array of css filepaths for the current entity type, keyed
   * by theme name.
   *
   * If provided, the "base" CSS file is included for all themes.
   * If a CSS file matching the current theme exists, it will also be included.
   *
   * @code
   * return array(
   *   'base' => drupal_get_path('module', 'test_module') . '/css/inline_entity_form.base.css',
   *   'seven' => drupal_get_path('module', 'test_module') . '/css/inline_entity_form.seven.css',
   * );
   * @endcode
   */
  public function css() {
    return array();
  }

  /**
   * Returns the default entity type labels.
   */
  public function defaultLabels() {
    $labels = array(
      'singular' => t('entity'),
      'plural' => t('entities'),
    );

    $info = entity_get_info($this->entityType);
    // Commerce and its contribs declare permission labels that can be used
    // for more precise and user-friendly strings.
    if (!empty($info['permission labels'])) {
      $labels = $info['permission labels'];
    }

    return $labels;
  }

  /**
   * Returns an array of entity type labels fit for display in the UI.
   */
  public function labels() {
    $labels = $this->defaultLabels();
    // The admin has specified the exact labels that should be used.
    if ($this->settings['override_labels']) {
      $labels = array(
        'singular' => $this->settings['label_singular'],
        'plural' => $this->settings['label_plural'],
      );
    }

    return $labels;
  }

  /**
   * Returns an array of fields used to represent an entity in the IEF table.
   *
   * The fields can be either Field API fields or properties defined through
   * hook_entity_property_info().
   *
   * Modules can alter the output of this method through
   * hook_inline_entity_form_table_fields_alter().
   *
   * @param $bundles
   *   An array of allowed bundles for this widget.
   *
   * @return
   *   An array of field information, keyed by field name. Allowed keys:
   *   - type: 'field' or 'property',
   *   - label: Human readable name of the field, shown to the user.
   *   - weight: The position of the field relative to other fields.
   *   Special keys for type 'field', all optional:
   *   - formatter: The formatter used to display the field, or "hidden".
   *   - settings: An array passed to the formatter. If empty, defaults are used.
   *   - delta: If provided, limits the field to just the specified delta.
   */
  public function tableFields($bundles) {
    $info = entity_get_info($this->entityType);
    $metadata = entity_get_property_info($this->entityType);

    $fields = array();
    if (!empty($info['entity keys']['label'])) {
      $label_key = $info['entity keys']['label'];
      $fields[$label_key] = array(
        'type' => 'property',
        'label' => $metadata ? $metadata['properties'][$label_key]['label'] : t('Label'),
        'weight' => 1,
      );
    }
    else {
      $id_key = $info['entity keys']['id'];
      $fields[$id_key] = array(
        'type' => 'property',
        'label' => $metadata ? $metadata['properties'][$id_key]['label'] : t('ID'),
        'weight' => 1,
      );
    }
    if (count($bundles) > 1) {
      $bundle_key = $info['entity keys']['bundle'];
      $fields[$bundle_key] = array(
        'type' => 'property',
        'label' => $metadata ? $metadata['properties'][$bundle_key]['label'] : t('Type'),
        'weight' => 2,
      );
    }

    return $fields;
  }

  /**
   * Returns a setting value.
   *
   * @param $name
   *   The name of the setting value to return.
   *
   * @return
   *   A setting value.
   */
  public function getSetting($name) {
    return $this->settings[$name];
  }

  /**
   * Returns an array of default settings in the form of key => value.
   */
  public function defaultSettings() {
    $defaults = array();
    $defaults['allow_new'] = TRUE;
    $defaults['allow_existing'] = FALSE;
    $defaults['match_operator'] = 'CONTAINS';
    $defaults['allow_clone'] = FALSE;
    $defaults['delete_references'] = FALSE;
    $labels = $this->defaultLabels();
    $defaults['override_labels'] = FALSE;
    $defaults['label_singular'] = $labels['singular'];
    $defaults['label_plural'] = $labels['plural'];

    return $defaults;
  }

  /**
   * Returns the settings form for the current entity type.
   *
   * The settings form is embedded into the IEF widget settings form.
   * Settings are later injected into the controller through $this->settings.
   *
   * @param $field
   *   The definition of the reference field used by IEF.
   * @param $instance
   *   The definition of the reference field instance.
   */
  public function settingsForm($field, $instance) {
    $labels = $this->labels();
    $states_prefix = 'instance[widget][settings][type_settings]';

    $form = array();
    $form['allow_new'] = array(
      '#type' => 'checkbox',
      '#title' => t('Allow users to add new @label.', array('@label' => $labels['plural'])),
      '#default_value' => $this->settings['allow_new'],
    );
    $form['allow_existing'] = array(
      '#type' => 'checkbox',
      '#title' => t('Allow users to add existing @label.', array('@label' => $labels['plural'])),
      '#default_value' => $this->settings['allow_existing'],
    );
    $form['match_operator'] = array(
      '#type' => 'select',
      '#title' => t('Autocomplete matching'),
      '#default_value' => $this->settings['match_operator'],
      '#options' => array(
        'STARTS_WITH' => t('Starts with'),
        'CONTAINS' => t('Contains'),
      ),
      '#description' => t('Select the method used to collect autocomplete suggestions. Note that <em>Contains</em> can cause performance issues on sites with thousands of nodes.'),
      '#states' => array(
        'visible' => array(
          ':input[name="' . $states_prefix . '[allow_existing]"]' => array('checked' => TRUE),
        ),
      ),
    );
    // The single widget doesn't offer autocomplete functionality.
    if ($instance['widget']['type'] == 'inline_entity_form_single') {
      $form['allow_existing']['#access'] = FALSE;
      $form['match_operator']['#access'] = FALSE;
    }

    $form['allow_clone'] = array(
      '#type' => 'checkbox',
      '#title' => t('Allow users to clone @label.', array('@label' => $labels['plural'])),
      '#default_value' => isset($this->settings['allow_clone'])? $this->settings['allow_clone'] : FALSE,
    );
    $form['delete_references'] = array(
      '#type' => 'checkbox',
      '#title' => t('Delete referenced @label when the parent entity is deleted.', array('@label' => $labels['plural'])),
      '#default_value' => $this->settings['delete_references'],
    );
    $form['override_labels'] = array(
      '#type' => 'checkbox',
      '#title' => t('Override labels'),
      '#default_value' => $this->settings['override_labels'],
    );
    $form['label_singular'] = array(
      '#type' => 'textfield',
      '#title' => t('Singular label'),
      '#default_value' => $this->settings['label_singular'],
      '#states' => array(
        'visible' => array(
          ':input[name="' . $states_prefix . '[override_labels]"]' => array('checked' => TRUE),
        ),
      ),
    );
    $form['label_plural'] = array(
      '#type' => 'textfield',
      '#title' => t('Plural label'),
      '#default_value' => $this->settings['label_plural'],
      '#states' => array(
        'visible' => array(
          ':input[name="' . $states_prefix . '[override_labels]"]' => array('checked' => TRUE),
        ),
      ),
    );

    return $form;
  }

  /**
   * Returns the entity type managed by this controller.
   *
   * @return
   *   The entity type.
   */
  public function entityType() {
    return $this->entityType;
  }

  /**
   * Returns the entity form to be shown through the IEF widget.
   *
   * When adding data to $form_state it should be noted that there can be
   * several IEF widgets on one master form, each with several form rows,
   * leading to possible key collisions if the keys are not prefixed with
   * $entity_form['#parents'].
   *
   * @param $entity_form
   *   The entity form.
   * @param $form_state
   *   The form state of the parent form.
   */
  public function entityForm($entity_form, &$form_state) {
    $info = entity_get_info($this->entityType);
    $entity = $entity_form['#entity'];

    if (!empty($info['fieldable'])) {
      $langcode = entity_language($this->entityType, $entity);
      field_attach_form($this->entityType, $entity, $entity_form, $form_state, $langcode);
    }

    $this->entityFormTranslation($entity_form);
    return $entity_form;
  }
  
  /**
   * Handles entity translation for inline entities.
   *
   * @param $entity_form
   *   The entity form.
   */
  protected function entityFormTranslation(&$entity_form) {
    if (($handler = module_invoke('entity_translation', 'get_handler', $this->entityType, $entity_form['#entity'])) && !$handler->isNewEntity()) {
      $form_langcode = $handler->getFormLanguage();
      $langcode = !empty($entity_form['#parent_language']) ? $entity_form['#parent_language'] : $handler->getLanguage();
      $translations = $handler->getTranslations();
      $update_langcode = $form_langcode && ($form_langcode != $langcode);
      $source = $handler->getSourceLanguage();
      $new_translation = !isset($translations->data[$form_langcode]);
  
      if (!isset($translations->data[$form_langcode]) || count($translations->data) > 1) {
        $handler->entityFormSharedElements($entity_form);
      }
  
      // If we are creating a new translation we need to retrieve form elements
      // populated with the source language values. In this case source values
      // have already been populated, so we need to preserve possible changes.
      // There might be situations, e.g. ajax calls, where the form language
      // has not been properly initialized before calling field_attach_form().
      // In this case we need to rebuild the form with the correct form language
      // and replace the field elements with the correct ones.
      // Borrowed some code from entity_translation_field_attach_form()
      // that won't get run because $form_state['rebuild'] is TRUE for all IEF
      // forms.
      if ($update_langcode || ($source && !isset($translations->data[$form_langcode]) && isset($translations->data[$source]))) {
        $entity_type = $entity_form['#entity_type'];
        list($id, , $bundle) = entity_extract_ids($entity_type, $entity_form['#entity']);
        foreach (field_info_instances($entity_type, $bundle) as $instance) {
  
          $field_name = $instance['field_name'];
          $field = field_info_field($field_name);
  
          // If we are creating a new translation we have to change the form
          // item language information from source to target language, this way
          // the user can find the form items already populated with the source
          // values while the field form element holds the correct language
          // information.
          if ($field['translatable']) {
            $element = &$entity_form[$field_name];
            $element['#entity_type'] = $entity_type;
            $element['#entity'] = $entity_form['#entity'];
            $element['#entity_id'] = $id;
            $element['#field_name'] = $field_name;
            $element['#source'] = $update_langcode ? $form_langcode : $source;
            $element['#previous'] = NULL;
            $element['#form_parents'] = $entity_form['#parents'];
  
            // If we are updating the form language we need to make sure that
            // the wrong language is unset and the right one is stored in the
            // field element (see entity_translation_prepare_element()).
            if ($update_langcode) {
              $element['#previous'] = $element['#language'];
              $element['#language'] = $form_langcode;
            }
  
            // Swap default values during form processing to avoid recursion.
            // We try to act before any other callback so that the correct
            // values are already in place for them.
            if (!isset($element['#process'])) {
              $element['#process'] = array();
            }
            array_unshift($element['#process'], 'entity_translation_prepare_element');
          }
        }
      }
      $entity_form['#element_validate'][] = 'entity_translation_entity_form_validate';
    }
  }

  /**
   * Validates the entity form.
   *
   * @param $entity_form
   *   The entity form.
   * @param $form_state
   *   The form state of the parent form.
   */
  public function entityFormValidate($entity_form, &$form_state) {
    $info = entity_get_info($this->entityType);
    $entity = $entity_form['#entity'];

    if (!empty($info['fieldable'])) {
      field_attach_form_validate($this->entityType, $entity, $entity_form, $form_state);
    }
  }

  /**
   * Handles the submission of an entity form.
   *
   * Prepares the entity stored in $entity_form['#entity'] for saving by copying
   * the values from the form to matching properties and, if the entity is
   * fieldable, invoking Field API submit.
   *
   * @param $entity_form
   *   The entity form.
   * @param $form_state
   *   The form state of the parent form.
   */
  public function entityFormSubmit(&$entity_form, &$form_state) {
    $info = entity_get_info($this->entityType);
    list(, , $bundle) = entity_extract_ids($this->entityType, $entity_form['#entity']);
    $entity = $entity_form['#entity'];
    $entity_values = &drupal_array_get_nested_value($form_state['values'], $entity_form['#parents']);

    // Copy top-level form values that are not for fields to entity properties,
    // without changing existing entity properties that are not being edited by
    // this form. Copying field values must be done using field_attach_submit().
    $values_excluding_fields = $info['fieldable'] ? array_diff_key($entity_values, field_info_instances($this->entityType, $bundle)) : $entity_values;
    foreach ($values_excluding_fields as $key => $value) {
      $entity->$key = $value;
    }
    
    // Update the entity language using the parent entity submitted value.
    if (!empty($info['entity keys']['language'])) {
      $parent_info = entity_get_info($entity_form['#parent_entity_type']);
      if (isset($parent_info['entity keys']['language']) && isset($form_state['values'][$parent_info['entity keys']['language']])) {
        $entity->{$info['entity keys']['language']} = $form_state['values'][$parent_info['entity keys']['language']];
        $handler = module_invoke('entity_translation', 'get_handler', $this->entityType, $entity);
        $lang = $handler->getFormLanguage();
        if ($lang == LANGUAGE_NONE) {
          if ($entity_form['#parent_language'] != LANGUAGE_NONE) {
            $handler->setFormLanguage($entity_form['#parent_language']);
          }
          else {
            $handler->setFormLanguage($form_state['values'][$parent_info['entity keys']['language']]);
          }
        }
      }
    }

    if ($info['fieldable']) {
      // Retrieve the current entity form language.
      $langcode = entity_language($this->entityType, $entity);
      
      // Handle a possible language change: new language values are inserted,
      // previous ones are deleted.
      foreach (field_info_instances($this->entityType, $bundle) as $instance) {
        $field_name = $instance['field_name'];
        $field = field_info_field($field_name);
        $previous_langcode = $entity_form[$field_name]['#language'];
        if ($field['translatable'] && $previous_langcode != $langcode) {
          $entity_values[$field_name][$langcode] = $entity_values[$field_name][$previous_langcode];
          $entity_values[$field_name][$previous_langcode] = array();
        }
      }
      
      field_attach_submit($this->entityType, $entity, $entity_form, $form_state);
      
      // Handle translation submission.
      $this->entityTranslationFormSubmit($entity_form, $entity);
      
    }
  }
  
  /**
   * Creates an entity translation if needed.
   */
  protected function entityTranslationFormSubmit($entity_form, $entity) {
    if (($handler = module_invoke('entity_translation', 'get_handler', $this->entityType, $entity)) && ($source = $handler->getSourceLanguage())) {
      list($id, , ) = entity_extract_ids($this->entityType, $entity);
      $translation = array(
          'entity_type' => $this->entityType,
          'entity_id' => $id,
          'language' => $handler->getFormLanguage(),
          'source' => $source,
          'status' => 1,
      );
      $handler->setTranslation($translation);
    }
  }

  /**
   * Returns the remove form to be shown through the IEF widget.
   *
   * @param $remove_form
   *   The remove form.
   * @param $form_state
   *   The form state of the parent form.
   */
  public function removeForm($remove_form, &$form_state) {
    $entity = $remove_form['#entity'];
    list($entity_id) = entity_extract_ids($this->entityType, $entity);
    $entity_label = entity_label($this->entityType, $entity);

    $remove_form['message'] = array(
      '#markup' => '<div>' . t('Are you sure you want to remove %label?', array('%label' => $entity_label)) . '</div>',
    );
    if (!empty($entity_id) && $this->getSetting('allow_existing')) {
      $access = entity_access('delete', $this->entityType, $entity);
      if ($access) {
        $labels = $this->labels();
        $remove_form['delete'] = array(
          '#type' => 'checkbox',
          '#title' => t('Delete this @type_singular from the system.', array('@type_singular' => $labels['singular'])),
        );
      }
    }

    return $remove_form;
  }

  /**
   * Handles the submission of a remove form.
   * Decides what should happen to the entity after the removal confirmation.
   *
   * @param $remove_form
   *   The remove form.
   * @param $form_state
   *   The form state of the parent form.
   *
   * @return int
   *   IEF_ENTITY_UNLINK or IEF_ENTITY_UNLINK_DELETE.
   */
  public function removeFormSubmit($remove_form, &$form_state) {
    $entity = $remove_form['#entity'];
    list($entity_id) = entity_extract_ids($this->entityType, $entity);
    $form_values = drupal_array_get_nested_value($form_state['values'], $remove_form['#parents']);
    // This entity hasn't been saved yet, we can just unlink it.
    if (empty($entity_id)) {
      return IEF_ENTITY_UNLINK;
    }
    // If existing entities can be referenced, the delete happens only when
    // specifically requested (the "Permanently delete" checkbox).
    if ($this->getSetting('allow_existing') && empty($form_values['delete'])) {
      return IEF_ENTITY_UNLINK;
    }

    return IEF_ENTITY_UNLINK_DELETE;
  }
  
  /**
   * Returns the remove translation form to be shown through the IEF widget.
   *
   * @param $remove_form
   *   The remove form.
   * @param $form_state
   *   The form state of the parent form.
   */
  public function removeTranslationForm($remove_form, &$form_state) {
    $entity = $remove_form['#entity'];
    list($entity_id) = entity_extract_ids($this->entityType, $entity);
    $entity_label = entity_label($this->entityType, $entity);
    $langcode = entity_language($this->entityType, $entity);
    $languages = language_list();
  
    $remove_form['message'] = array(
        '#markup' => '<div>' . t('Are you sure you want to remove the @language_label translation for %label?', array(
            '%label' => $entity_label,
            '@language_label' => isset($languages[$langcode]) ? $languages[$langcode]->name : $langcode,
        )) . '</div>',
    );
  
    return $remove_form;
  }

  /**
   * Creates a clone of the given entity.
   *
   * Copies the entity_ui_clone_entity() approach, extending it to unset
   * additional unique properties.
   *
   * @param $entity
   *   The entity to clone.
   *
   * @return
   *   The unsaved cloned entity.
   */
  public function createClone($entity) {
    $cloned_entity = clone $entity;
    $cloned_entity->is_new = TRUE;

    $entity_info = entity_get_info($this->entityType);
    // Make sure the status of a cloned exportable is custom.
    if (!empty($entity_info['exportable'])) {
      $status_key = isset($entity_info['entity keys']['status']) ? $entity_info['entity keys']['status'] : 'status';
      $cloned_entity->$status_key = ENTITY_CUSTOM;
    }
    // Unset properties specified as entity keys.
    foreach (array('id', 'name', 'revision') as $key) {
      if (!empty($entity_info['entity keys'][$key])) {
        $cloned_entity->{$entity_info['entity keys'][$key]} = NULL;
      }
    }
    // Unset other common properties.
    foreach (array('created', 'changed', 'uid', 'revision_timestamp', 'revision_uid') as $property) {
      if (isset($cloned_entity->{$property})) {
        $cloned_entity->{$property} = NULL;
      }
    }

    return $cloned_entity;
  }

  /**
   * Permanently saves the given entity.
   *
   * @param $entity
   *   The entity to save.
   * @param array $context
   *   Available keys:
   *   - parent_entity_type: The type of the parent entity.
   *   - parent_entity: The parent entity.
   */
  public function save($entity, $context) {
    entity_save($this->entityType, $entity);
  }

  /**
   * Delete permanently saved entities.
   *
   * @param $ids
   *   An array of entity IDs.
   * @param array $context
   *   Available keys:
   *   - parent_entity_type: The type of the parent entity.
   *   - parent_entity: The parent entity.
   */
  public function delete($ids, $context) {
    entity_delete_multiple($this->entityType, $ids);
  }
}
