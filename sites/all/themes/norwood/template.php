<?php
function norwood_preprocess_page(&$variables) {
  drupal_add_css(path_to_theme().'/superfish.css', 'theme');
  drupal_add_js(path_to_theme().'/superfish.js', 'theme');
  $variables['scripts'] = drupal_get_js();
  $variables['styles'] = drupal_get_css();
}