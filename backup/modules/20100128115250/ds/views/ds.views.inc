<?php
// $Id: ds.views.inc,v 1.1.2.2 2009/12/26 13:39:53 swentel Exp $

/**
 * @file
 * Views file for ds.
 */

/**
 * Implementation of hook_views_plugins().
 */
function ds_views_plugins() {
  $path = drupal_get_path('module', 'ds');
  return array(
    'module' => 'ds', // This just tells our themes are elsewhere.
    'row' => array(
      'ds' => array(
        'title' => t('Display suite'),
        'help' => t('Display the object through the Display suite module.'),
        'handler' => 'views_plugin_ds_object_view',
        'theme' => 'ds_row_object',
        'path' => $path .'/views',
        'theme path' => $path .'/theme',
        'theme file' => 'theme.inc',
        'uses options' => TRUE,
        'type' => 'normal',
        'help topic' => 'style-node',
      ),
    ),
  );
}
