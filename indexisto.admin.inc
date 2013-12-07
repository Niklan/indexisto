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

  // Мы оповещаем пользователя о том, что настройки могут ухудшить ситуацию.
  drupal_set_message('Данные настройки предназначены для продвинутых пользователей. Если вы не пониманимаете что они делают или как ими пользоваться, пожалуйста, не включайте отсылку данных, так как это может навредить.', 'warning', TRUE);

  // Получаем все типы содержимого на сайте.
  $content_types = node_type_get_types();

  $form['indexisto_send_data'] = array(
    '#type' => 'checkbox',
    '#title' => 'Отсылать структурированные данные',
    '#description' => 'Включая данную опцию, при сохранении, изменении или удалении содержимого, на сервер API будут отсылаться структурированные данные, для более точного индексирования.',
    '#default_value' => variable_get('indexisto_send_data', 0),
    '#weight' => 0,
  );

  // Секретный ключ
  $form['indexisto_secret_api_key'] = array(
    '#type' => 'textfield',
    '#title' => 'Секретный ключ API',
    '#default_value' => variable_get('indexisto_secret_api_key', ''),
    '#weight' => 1,
    '#required' => TRUE,
  );


  $form['indexisto_path_aliases'] = array(
    '#type' => 'checkbox',
    '#title' => 'По возможности использовать синонимы',
    '#description' => 'Если выключить, то будут отсылаться системные url вида /node/1.',
    '#default_value' => variable_get('indexisto_path_aliases', 1),
    '#weight' => 2,
  );

  // Типы содержимого.
  $form['content_types'] = array(
    '#type' => 'fieldset',
    '#title' => 'Типы содержимого',
    '#weight' => 3,
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

    // Отсылать ли данные по данному типу содержимому или нет
    // 0 - отсылать.
    // 1 - запрет на отсылку.
    $form['content_types'][$content_type]['indexisto_' . $content_type . '_send_data'] = array(
      '#type' => 'checkbox',
      '#title' => 'Запретить индексацию данного типа содержимого',
      '#description' => 'Данный тип материала не попадет в индекс.',
      '#default_value' => variable_get('indexisto_' . $content_type . '_send_data', 0),
      '#weight' => -1,
    );

    // Поле для заголовка.
    $form['content_types'][$content_type]['indexisto_' . $content_type . '_title'] = array(
      '#type' => 'textfield',
      '#title' => 'Заголовок содержимого',
      '#description' => 'Возвращаемый заголовок содержимого.',
      '#default_value' => variable_get('indexisto_' . $content_type . '_title', '[node:title]'),
      '#weight' => 0,
      '#token_insert' => TRUE,
    );

    // Содержимое.
    $form['content_types'][$content_type]['indexisto_' . $content_type . '_body'] = array(
      '#type' => 'textfield',
      '#title' => 'Содержимое',
      '#description' => 'Содержание. Рекомендуется [node:body].',
      '#default_value' => variable_get('indexisto_' . $content_type . '_body', '[node:body]'),
      '#weight' => 1,
      '#token_insert' => TRUE,
    );

    // Общее содержимое.
    $form['content_types'][$content_type]['indexisto_' . $content_type . '_sumtext'] = array(
      '#type' => 'textfield',
      '#title' => 'Общее содержимое',
      '#description' => 'Сборка всех контентных данных в единое место, по ним будет производиться поиск. Рекомендуется вставка содержимого, тегов и прочих контентовых данных.',
      '#default_value' => variable_get('indexisto_' . $content_type . '_sumtext', '[node:body]'),
      '#weight' => 2,
      '#token_insert' => TRUE,
    );

    // Изображение.
    $form['content_types'][$content_type]['indexisto_' . $content_type . '_image'] = array(
      '#type' => 'textfield',
      '#title' => 'Изображение',
      '#description' => 'Изображение. Рекомендуется [node:body].',
      '#default_value' => variable_get('indexisto_' . $content_type . '_image', ''),
      '#weight' => 3,
      '#token_insert' => TRUE,
    );

    // Теги.
    $form['content_types'][$content_type]['indexisto_' . $content_type . '_tags'] = array(
      '#type' => 'textfield',
      '#title' => 'Теги',
      '#description' => 'Укажите поле с тегами.',
      '#default_value' => variable_get('indexisto_' . $content_type . '_tags', ''),
      '#weight' => 4,
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

  // Комментарии.
  $form['comments'] = array(
    '#type' => 'fieldset',
    '#title' => 'Комментарии',
    '#weight' => 4,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  // Запрет индексации.
  // 0 - разрешено.
  // 1 - запрещено.
  $form['comments']['indexisto_comments_send_data'] = array(
    '#type' => 'checkbox',
    '#title' => 'Запретить индексацию комментариев.',
    '#description' => 'Данный тип материала не попадет в индекс.',
    '#default_value' => variable_get('indexisto_comments_send_data', 0),
    '#weight' => -1,
  );

  $form['comments']['indexisto_comments_body'] = array(
    '#type' => 'textfield',
    '#title' => 'Содержимое комментария',
    '#description' => 'Содержание. Рекомендуется [comment:body].',
    '#default_value' => variable_get('indexisto_comments_body', '[comment:body]'),
    '#weight' => 1,
    '#token_insert' => TRUE,
  );

  // Добавляем списки токенов.
  $form['comments']['token_help'] = array(
    '#title' => t('Replacement patterns'),
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#weight' => 99,
  );

  $form['comments']['token_help']['help'] = array(
    '#theme' => 'token_tree',
    '#token_types' => array(
      'comment',
    ),
  );

  return system_settings_form($form);
}

/**
 * @param $form
 * @param $form_state
 *
 * Настройка батч операций.
 */
function indexisto_batch_form($form, &$form_state) {
  $form = array();

  // Типы наших операций.
  $operation_types = array(
    0 => 'Добавление и\или обновление в индекс',
    1 => 'Удаление из индекса'
  );

  $form['batch_operation_type'] = array(
    '#type' => 'radios',
    '#title' => 'Тип операции на выполнение',
    '#default_value' => 0,
    '#options' => $operation_types,
    '#description' => 'Выберите какие операции необходимо произвести.',
  );

  // Доступные для операций типы.
  $allowed_types = array();

  // Получаем все типы содержимого на сайте.
  $content_types = node_type_get_types();

  // Добавляем в доступные типы все наши типы содержимого.
  foreach ($content_types as $content_type) {
    $allowed_types[] = $content_type->type;
  }

  // Не забываем и про комментарии.
  $allowed_types[] = 'comment';

  $form['batch_operation_content_types'] = array(
    '#type' => 'checkboxes',
    '#options' => drupal_map_assoc($allowed_types),
    '#title' => 'Над какими типами проделать операцию',
    '#required' => TRUE,
  );

  if (variable_get('indexisto_send_data', 0) && variable_get('indexisto_secret_api_key', 0)) {
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => 'Выполнить',
    );
  }
  else {
    form_set_error('indexisto_batch_form', 'Для того чобы воспользоваться данной формой, необходимо включить отправку структурированных данных и указать ключ API.');
  }

  return $form;
}

/**
 * @param $form
 * @param $form_state
 *
 * Перехватываем подтверждение формы батч операций.
 */
function indexisto_batch_form_submit($form, &$form_state) {
  indexisto_batch_start($form, $form_state);
}