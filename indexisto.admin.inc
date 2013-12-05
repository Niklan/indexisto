<?php
/**
 * @file
 * Административный интерфейс модуля.
 */

/**
 * @param $form
 * @param $form_state
 * @return mixed
 *
 * Форма базовой настройки модуля.
 */
function indexisto_settings_form($form, &$form_state) {
  $form = array();

  $form['indexisto_index_id'] = array(
    '#type' => 'textfield',
    '#title' => 'ID поисковой системы',
    '#description' => 'Укажите ID вашей поисковой системы.',
    '#default_value' => variable_get('indexisto_index_id', ''),
    '#size' => 60,
    '#maxlength' => 128,
    '#required' => TRUE,
  );

  $form['indexisto_autocomplete'] = array(
    '#type' => 'checkbox',
    '#title' => 'Автозаполнение',
    '#description' => 'Отметьте если необходимо автозаполнение.',
    '#default_value' => variable_get('indexisto_autocomplete', 0),
  );

  $form['indexisto_autocorrect'] = array(
    '#type' => 'checkbox',
    '#title' => 'Автокоррекция',
    '#description' => 'Отметьте если необходимо автозаполнение.',
    '#default_value' => variable_get('indexisto_autocorrect', 0),
  );

  $form['indexisto_placeholder'] = array(
    '#type' => 'textfield',
    '#title' => 'Текст в строке поиска',
    '#description' => 'Текст который отображается до начала ввода поискового запроса.',
    '#default_value' => variable_get('indexisto_placeholder', 'Начните ввод'),
    '#size' => 60,
    '#maxlength' => 128,
    '#required' => TRUE,
  );

  $form['indexisto_input_style'] = array(
    '#type' => 'textfield',
    '#title' => 'Инлайн стили для поля поиска',
    '#description' => 'Укажите собственные стили, если это необходимо.',
    '#default_value' => variable_get('indexisto_input_style', INDEXISTO_DEFAULT_STYLES),
    '#size' => 60,
    '#maxlength' => 1000,
    '#required' => TRUE,
  );

  return system_settings_form($form);
}

/**
 * @param $form
 * @param $form_state
 *
 * Детальная настройка под контент.
 */
function indexisto_content_settings($form, &$form_state) {
  $form = array();

  // Получаем все типы содержимого на сайте.
  $content_types = node_type_get_types();

  // Типы содержимого.
  $form['content_types'] = array(
    '#type' => 'fieldset',
    '#title' => 'Типы содержимого',
    '#weight' => 0,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  foreach ($content_types as $content_type => $content_type_info) {
    // Генерируем основную обертку для настроек определенного типа содержимого.
    $form['content_types'][$content_type] = array(
      '#type' => 'fieldset',
      '#title' => $content_type_info->name . ' (' . $content_type . ')',
      '#weight' => 0,
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );

    // Поле для заголовка.
    $form['content_types'][$content_type]['indexisto_' . $content_type . '_title'] = array(
      '#type' => 'textfield',
      '#title' => 'Заголовок содержимого',
      '#description' => 'Возвращаемый заголовок содержимого. Рекомендуется [node:title].',
      '#default_value' => variable_get('indexisto_' . $content_type . '_title', ''),
      '#weight' => 0,
      '#token_insert' => TRUE,
    );

    // Содержимое.
    $form['content_types'][$content_type]['indexisto_' . $content_type . '_body'] = array(
      '#type' => 'textfield',
      '#title' => 'Содержимое',
      '#description' => 'Содержание. Рекомендуется [node:body].',
      '#default_value' => variable_get('indexisto_' . $content_type . '_body', ''),
      '#weight' => 1,
      '#token_insert' => TRUE,
    );

    // Изображение.
    $form['content_types'][$content_type]['indexisto_' . $content_type . '_image'] = array(
      '#type' => 'textfield',
      '#title' => 'Изображение',
      '#description' => 'Изображение. Рекомендуется [node:body].',
      '#default_value' => variable_get('indexisto_' . $content_type . '_image', ''),
      '#weight' => 2,
      '#token_insert' => TRUE,
    );

    // Добавляем списки токенов.
    $form['content_types'][$content_type]['token_help'] = array(
      '#title' => t('Replacement patterns'),
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#weight' => 99,
    );
    $form['content_types'][$content_type]['token_help']['help'] = array(
      '#theme' => 'token_tree',
      '#token_types' => array(
        'node',
      ),
    );
  }

  return system_settings_form($form);
}
