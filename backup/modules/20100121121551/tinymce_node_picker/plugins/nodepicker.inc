<?php
// $id$

/**
 * @file
 * Wysiwyg API integration on behalf of TinyMCE Node Picker module.
 */

/**
 * Implementation of hook_wysiwyg_plugin().
 */
function tinymce_node_picker_nodepicker_plugin() {
	$plugins['nodepicker'] = array(
    'title' => t('Node picker'),
    'vendor url' => 'http://drupal.org/project/tinymce_node_picker',
    'icon file' => 'nodepicker.png',
    'icon title' => t('Insert link to internal content'),
    'settings' => array(
    	'dialog' => array(
    		'title' => t('Test'),
				'url' => base_path() . 'index.php?q=admin/tinymce-node-picker',
				'width' => 640,
				'height' => 480,
			),
    ),
  );
  return $plugins;
}