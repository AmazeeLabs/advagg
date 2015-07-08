<?php

/**
 * @file
 * Admin page callbacks for the advagg font module.
 */

/**
 * Form builder; Configure advagg settings.
 *
 * @ingroup forms
 *
 * @see system_settings_form()
 */
function advagg_font_admin_settings_form() {
  drupal_set_title(t('AdvAgg: Async Font Loader'));

  $form = array();

  $form['advagg_font_fontfaceobserver'] = array(
    '#type' => 'radios',
    '#title' => t('Use font face observer to load fonts asynchronously.'),
    '#default_value' => variable_get('advagg_font_fontfaceobserver', ADVAGG_FONT_FONTFACEOBSERVER),
    '#options' => array(
      0 => t('Disabled'),
//       2 => t('Inline javascript'),
//       4 => t('Local file included in aggregate'),
      6 => t('Externally load the latest from github'),
    ),
    '#description' => t('This will use the fallback font until the font has been downloaded. See <a href="@link">fontfaceobserver</a> for more info.', array(
      '@link' => 'https://github.com/bramstein/fontfaceobserver',
    )),
  );

  // Get all css files and scan for quoted fonts.
  $form['fonts'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#title' => t('CSS files with external fonts'),
  );
  // Get filename, filename_hash, and changes.
  $results = db_select('advagg_files', 'af')
    ->fields('af', array('filename', 'filename_hash', 'changes'))
    ->condition('filetype', 'css')
    ->execute();
  while ($row = $results->fetchAssoc()) {
    if (!file_exists($row['filename'])) {
      continue;
    }
    // Get the file contents.
    $file_contents = file_get_contents($row['filename']);
    // Get font names.
    $replacements = advagg_font_get_replacements_array($file_contents);
    if (!empty($replacements)) {
      $fonts = array();
      foreach ($replacements as $replacement) {
        $fonts[$replacement[3]] = $replacement[5];
      }
      $form['fonts'][$row['filename_hash']] = array(
        '#markup' => '<div>' . t('%file - <code>@replacements</code><br />', array(
          '@replacements' => str_ireplace('array', '', print_r($fonts, TRUE)),
          '%file' => $row['filename'],
        )) . '</div>',
      );
    }
  }
  $children = element_children($form['fonts']);

  // If no fonts are found; disable this module.
  if (count($children) == 0) {
    $form['advagg_font_fontfaceobserver']['#default_value'] = 0;
    $form['advagg_font_fontfaceobserver']['#disabled'] = TRUE;

    $form['fonts'] = array(
      '#type' => 'fieldset',
      '#title' => t('No CSS files with external fonts found.'),
      '#description' => t('Currently this module is not doing anything. Recommend uninstalling it as advagg is not processing any css files that use an external font file.'),
    );

  }

  // Clear the cache bins on submit.
  $form['#submit'][] = 'advagg_font_admin_settings_form_submit';

  return system_settings_form($form);
}

// Submit callback.
/**
 * Clear out the advagg cache bin when the save configuration button is pressed.
 */
function advagg_font_admin_settings_form_submit($form, &$form_state) {
  $cache_bins = advagg_flush_caches();
  foreach ($cache_bins as $bin) {
    cache_clear_all('*', $bin, TRUE);
  }
}
