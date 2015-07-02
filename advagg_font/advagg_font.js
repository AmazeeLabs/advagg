/**
 * @file
 * Used to add a class to the top level element when an external font is ready.
 */

/* global Drupal:false */

/**
 * Run the check.
 *
 * @param string key
 *   The class name to add to the html tag.
 * @param string value
 *   The font name.
 */
function advagg_run_check(key, value) {
  new window.FontFaceObserver(value).check().then(function () {
    // Set Class.
    window.document.documentElement.className += ' ' + key;

    // Set Cookie for a day.
    expire_date = new Date(new Date().getTime() + 86400 * 1000);
    document.cookie = 'advaggfont_' + key + '=' + value + ';'
      + ' expires=' + expire_date.toGMTString() + ';'
      + ' path=/;'
      + ' domain=.' + document.location.hostname + ';';
  }, function() {});
}

/**
 * Get the list of fonts to check for.
 */
function advagg_font_add_font_classes_on_load() {
  for (var key in Drupal.settings.advagg_font) {
    var cookie_pos = document.cookie.indexOf('advaggfont_' + key + '=' + Drupal.settings.advagg_font[key]);
    if (cookie_pos === -1) {
      // Wait till the font is downloaded, then set cookie.
      advagg_run_check(key, Drupal.settings.advagg_font[key]);
    }
  }
}

/**
 * Make sure FontFaceObserver and Drupal.settings are defined  before running.
 */
function advagg_font_check() {
  if (window.FontFaceObserver && window.jQuery && window.Drupal && window.Drupal.settings) {
    advagg_font_add_font_classes_on_load();
  }
  else {
    window.setTimeout(advagg_font_check, 20);
  }
}

// Start the process.
advagg_font_check();

// Check cookies ASAP and set class.
var fonts = document.cookie.split('advagg');
for (var key in fonts) {
  var font = fonts[key].split('=');
  var pos = font[0].indexOf('font_');
  if (pos !== -1) {
    window.document.documentElement.className += ' ' + font[0].substr(5);
  }
}
