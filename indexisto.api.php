<?php
/**
 * @file
 *
 * Описание всего API что предоставляет модуль.
 */

/**
 * Используем hook_indexisto_node_data_prepare_alter().
 *
 * Получает JSON данные после подготовки ноды, которые затем отправляются в
 * индекс.
 */
function hook_indexisto_node_data_prepare_alter(&$data) {
  // Do some stuff.

  return $data;
}

/**
 * Используем hook_indexisto_comment_data_prepare_alter().
 *
 * Получает JSON данные после подготовки комментария, которые затем отправляются
 * в индекс.
 */
function hook_indexisto_comment_data_prepare_alter(&$data) {
  // Do some stuff.

  return $data;
}
