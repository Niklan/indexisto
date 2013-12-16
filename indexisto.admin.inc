<?php
/**
 * @file
 * Administrative interface..
 */

/**
 * Basic settings form.
 */
function indexisto_settings_form($form, &$form_state) {
  $form = array();

  $form['indexisto_index_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Search index ID'),
    '#description' => t('ID of your search index.'),
    '#default_value' => variable_get('indexisto_index_id', ''),
    '#size' => 60,
    '#maxlength' => 128,
    '#required' => TRUE,
    '#weight' => 0,
  );

  // Secret key.
  $form['indexisto_secret_api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('API secret'),
    '#default_value' => variable_get('indexisto_secret_api_key', ''),
    '#weight' => 1,
    '#required' => TRUE,
  );

  $form['indexisto_send_data'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable indexation'),
    '#description' => t('On\off content indexation.'),
    '#default_value' => variable_get('indexisto_send_data', 0),
    '#weight' => 2,
  );

  return system_settings_form($form);
}

/**
 * Content settings for indexation.
 */
function indexisto_content_settings($form, &$form_state) {
  $form = array();

  // We warn user, cuz change this settings can broke indexation.
  drupal_set_message(t('These settings are for advanced users. If you do not understand what they do or how to use them, please do not change anything, since it can hurt.'), 'warning', TRUE);

  // Get all content types.
  $content_types = node_type_get_types();

  $form['indexisto_path_aliases'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use path aliases'),
    '#description' => t("Enable this option for using path aliases for URL's."),
    '#default_value' => variable_get('indexisto_path_aliases', 1),
    '#weight' => 2,
  );

  // Content types.
  $form['content_types'] = array(
    '#type' => 'fieldset',
    '#title' => t('Content types'),
    '#weight' => 3,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  foreach ($content_types as $content_type => $content_type_info) {
    // Generate main fieldset for each content type.
    $form['content_types'][$content_type] = array(
      '#type' => 'fieldset',
      '#title' => $content_type_info->name . ' (' . $content_type . ')',
      '#weight' => 0,
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );

    // Send data of current content type or not.
    // 0 - send enabled.
    // 1 - disabled.
    $form['content_types'][$content_type]['indexisto_' . $content_type . '_send_data'] = array(
      '#type' => 'checkbox',
      '#title' => t('Block this content type for indexation.'),
      '#description' => t('This content type will not be indexed.'),
      '#default_value' => variable_get('indexisto_' . $content_type . '_send_data', 0),
      '#weight' => -1,
    );

    // Title field.
    $form['content_types'][$content_type]['indexisto_' . $content_type . '_title'] = array(
      '#type' => 'textfield',
      '#title' => t('Content title'),
      '#description' => t('Returns content title.'),
      '#default_value' => variable_get('indexisto_' . $content_type . '_title', '[node:title]'),
      '#weight' => 0,
      '#token_insert' => TRUE,
    );

    // Content.
    $form['content_types'][$content_type]['indexisto_' . $content_type . '_body'] = array(
      '#type' => 'textfield',
      '#title' => t('Content'),
      '#description' => t('Main content of this content type.'),
      '#default_value' => variable_get('indexisto_' . $content_type . '_body', '[node:body]'),
      '#weight' => 1,
      '#token_insert' => TRUE,
    );

    // Summary text.
    $form['content_types'][$content_type]['indexisto_' . $content_type . '_sumtext'] = array(
      '#type' => 'textfield',
      '#title' => t('Summary content'),
      '#description' => t('In this field we add all needed content data from current content-type. This data will be used for search.'),
      '#default_value' => variable_get('indexisto_' . $content_type . '_sumtext', '[node:body] [node:field-tags]'),
      '#weight' => 2,
      '#token_insert' => TRUE,
    );

    // Image.
    $form['content_types'][$content_type]['indexisto_' . $content_type . '_image'] = array(
      '#type' => 'textfield',
      '#title' => t('Image'),
      '#description' => t('Main image for search results.'),
      '#default_value' => variable_get('indexisto_' . $content_type . '_image', '[node:field-image]'),
      '#weight' => 3,
      '#token_insert' => TRUE,
    );

    // Tags.
    $form['content_types'][$content_type]['indexisto_' . $content_type . '_tags'] = array(
      '#type' => 'textfield',
      '#title' => t('Tags'),
      '#description' => t('Field with tags of current content type.'),
      '#default_value' => variable_get('indexisto_' . $content_type . '_tags', '[node:field-tags]'),
      '#weight' => 4,
      '#token_insert' => TRUE,
    );

    // Add token list.
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

  // Comments.
  $form['comments'] = array(
    '#type' => 'fieldset',
    '#title' => t('Comments'),
    '#weight' => 4,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  // Block comment from indexation.
  // 0 - allowed.
  // 1 - blocked.
  $form['comments']['indexisto_comments_send_data'] = array(
    '#type' => 'checkbox',
    '#title' => t('Disallow comments from indexation.'),
    '#description' => t('Comments will not be indexed.'),
    '#default_value' => variable_get('indexisto_comments_send_data', 0),
    '#weight' => -1,
  );

  $form['comments']['indexisto_comments_body'] = array(
    '#type' => 'textfield',
    '#title' => t('Comment content'),
    '#description' => t('Main content of the comment.'),
    '#default_value' => variable_get('indexisto_comments_body', '[comment:body]'),
    '#weight' => 1,
    '#token_insert' => TRUE,
  );

  // Add token list.
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
 * Settings for batch operations.
 */
function indexisto_batch_form($form, &$form_state) {
  $form = array();

  // Operation types.
  $operation_types = array(
    0 => t('Add\update index data.'),
    1 => t('Remove from index.'),
  );

  $form['batch_operation_type'] = array(
    '#type' => 'radios',
    '#title' => t('Operation type'),
    '#default_value' => 0,
    '#options' => $operation_types,
    '#description' => t('Which operation will be performed.'),
  );

  $form['info'] = array(
    '#type' => 'fieldset',
    '#collapsible' => FALSE,
    '#title' => t('Help'),
    '#description' => t('Choose content for doing operation. Comments are indexed separately from the content.'),
  );

  // Allowed types for operations.
  $allowed_types = array();

  // Get all content types.
  $content_types = node_type_get_types();

  // Add all found content types to array;
  foreach ($content_types as $content_type) {
    $allowed_types[] = $content_type->type;
  }

  $form['batch_operation_content_types'] = array(
    '#type' => 'checkboxes',
    '#options' => drupal_map_assoc($allowed_types),
    '#title' => t('Content types'),
  );

  $form['batch_operation_comments'] = array(
    '#type' => 'checkboxes',
    '#options' => drupal_map_assoc(array('comments')),
    '#title' => t('Comments'),
  );

  if (variable_get('indexisto_send_data', 0) && variable_get('indexisto_secret_api_key', 0)) {
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Submit'),
    );
  }
  else {
    form_set_error('indexisto_batch_form', t('In order to use this form, you must enable the sending of structured data and specify the key API.'));
  }

  return $form;
}

/**
 * Submit of our batch form.
 */
function indexisto_batch_form_submit($form, &$form_state) {
  // Has selected content.
  $is_content = FALSE;
  foreach ($form_state['values']['batch_operation_content_types'] as $types) {
    if ($types != '0') {
      $is_content = TRUE;
    }
  }

  $is_comment = $form_state['values']['batch_operation_comments']['comments'] ? TRUE : FALSE;

  // If selected content and comments we return error.
  if ($is_comment && $is_content) {
    drupal_set_message('Please select either the content types or comments.', 'error');
  }
  elseif (!$is_comment && !$is_content) {
    drupal_set_message('Please select either the content types or comments', 'error');
  }
  // If comments, we must to adjust the data.
  elseif ($is_comment && !$is_content) {
    $form_state['values']['batch_operation_content_types'] = array('comment' => 'comment');
    indexisto_batch_start($form, $form_state);
  }
  else {
    indexisto_batch_start($form, $form_state);
  }

}

/**
 * First time activation form.
 */
function indexisto_settings_first_time($form, &$form_state) {
  $form = array();

  $form['register'] = array(
    '#type' => 'fieldset',
    '#title' => t('Register'),
    '#description' => t('If you do not have an account in indexisto, you can specify here for your information and you will automatically create an account, search index and all the settings for the correct operation of the module.'),
  );

  $form['register']['register_agree'] = array(
    '#type' => 'checkbox',
    '#title' => t('I want to register an account'),
    '#description' => t('Tick ​​this box to you account will be created automatically with all the necessary settings.'),
  );

  $form['register']['register_email'] = array(
    '#type' => 'textfield',
    '#title' => t('E-Mail'),
    '#description' => t('Specify the E-Mail which create an account.'),
    '#default_value' => variable_get('site_mail', ''),
  );

  $form['register']['register_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('Secret key'),
    '#description' => t('Specify the key to be used as a secret for the index.'),
    '#default_value' => drupal_random_key('15'),
  );

  $form['auth'] = array(
    '#type' => 'fieldset',
    '#title' => t('Authorization'),
    '#description' => 'If you have already set up an account on the index and indexisto, fill in the appropriate fields.',
  );

  $form['auth']['auth_agree'] = array(
    '#type' => 'checkbox',
    '#title' => t('I want to use existing settings.'),
    '#description' => t('Tick ​​this box, we will use the existing settings. You will be able to correct them in the future.'),
  );

  $form['auth']['index_id'] = array(
    '#type' => 'textfield',
    '#title' => t('ID search engine'),
    '#description' => t('Specify the ID of your search engine.'),
    '#default_value' => variable_get('indexisto_index_id', ''),
    '#size' => 60,
    '#maxlength' => 128,
  );

  $form['auth']['auth_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('Secret key'),
    '#description' => t('Specify the key to be used as a secret for the index.'),
    '#default_value' => '',
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  return $form;
}

/**
 * First time settings form submit.
 */
function indexisto_settings_first_time_submit($form, &$form_state) {
  // User want to register.
  $is_register = $form_state['values']['register_agree'];

  // User want to authorize.
  $is_auth = $form_state['values']['auth_agree'];

  // User must select only one of it.
  if ($is_auth && $is_register) {
    drupal_set_message(t('Please decide you want to log in or register.'), 'error');
  }
  // User cant select nothing.
  elseif (!$is_auth && !$is_register) {
    drupal_set_message(t('Please decide you want to log in or register.'), 'error');
  }

  // If user want to register.
  if ($is_register && !$is_auth) {
    $email = $form_state['values']['register_email'];
    $secret = $form_state['values']['register_secret'];

    if (empty($email) || empty($secret)) {
      drupal_set_message(t('E-Mail field and secret are required'), 'error');
    }
    else {
      $options = array(
        'method' => 'POST',
      );

      $host = INDEXISTO_INDEX_DOMAIN_NAME;
      $email = $form_state['values']['register_email'];
      $secret = $form_state['values']['register_secret'];

      $request_url = 'http://adminpanel.indexisto.com/adminpanel/apireg.rpc?host=' . $host . '&email=' . $email . '&secret=' . $secret;
      $result = drupal_http_request($request_url, $options);

      // If alright with request we finalize the data.
      if ($result->code == 200) {
        // Enable send data and specify default data.
        if (!empty($secret) && !empty($result->data)) {
          variable_set('indexisto_send_data', 1);
          variable_set('indexisto_secret_api_key', $secret);
          variable_set('indexisto_index_id', $result->data);
        }

        // Remove 'first time' installation mark.
        variable_set('indexisto_first_time', FALSE);
        // Clear cache for new menu items.
        menu_rebuild();
        // Congratulations.
        drupal_set_message(t('You have successfully registered on indexisto.'), 'status');
      }
      else {
        drupal_set_message(t('Something went wrong with the registration of a new account. Perhaps you already have an account and you should be logged.'), 'error');
      }
    }
  }

  // If user already have an account.
  if ($is_auth && !$is_register) {
    $index_id = $form_state['values']['index_id'];
    $secret = $form_state['values']['auth_secret'];

    // We just need one index id.
    if (empty($index_id)) {
      drupal_set_message(t('For authorization must specify at least the index ID.'), 'error');
    }
    else {
      variable_set('indexisto_index_id', $index_id);

      // If secret specify too, we enable sending data.
      if (!empty($secret)) {
        variable_set('indexisto_send_data', 1);
        variable_set('indexisto_secret_api_key', $secret);
      }

      // Remove 'first time' mark.
      variable_set('indexisto_first_time', FALSE);
      // Clear cache for new menu items.
      menu_rebuild();
    }
  }
}


/**
 * Style settings form.
 */
function indexisto_style($form, &$form_state) {
  $form = array();

  $form['indexisto_placeholder'] = array(
    '#type' => 'textfield',
    '#title' => t('Placeholder text'),
    '#description' => t('The text that is displayed before entering the search query.'),
    '#default_value' => variable_get('indexisto_placeholder', t('Start typing')),
    '#size' => 60,
    '#maxlength' => 128,
    '#required' => TRUE,
  );

  $form['indexisto_input_style'] = array(
    '#type' => 'textfield',
    '#title' => t('Inline CSS styles for search input'),
    '#description' => t('Specify your own styles, if necessary.'),
    '#default_value' => variable_get('indexisto_input_style', INDEXISTO_DEFAULT_STYLES),
    '#size' => 60,
    '#maxlength' => 1000,
    '#required' => TRUE,
  );

  return system_settings_form($form);
}
