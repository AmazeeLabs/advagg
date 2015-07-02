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
