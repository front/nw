<?php
// $Id: views_plugin_ds_object_view.inc,v 1.1.2.4 2009/12/28 15:50:07 swentel Exp $

/**
 * @file
 * Contains the nd view style plugin.
 */

/**
 * Plugin which performs a object on the resulting object based on ds build modes.
 */
class views_plugin_ds_object_view extends views_plugin_row {

  function option_definition() {
    $options = parent::option_definition();
    $options['build_mode'] = array('default' => 'teaser');
    $options['advanced'] = FALSE;
    return $options;
  }

  function options_form(&$form, &$form_state) {
    $views_base_table = $this->view->base_table;
    $build_mode_options = array();
    $build_modes = ds_get_build_modes();
    foreach ($build_modes as $module => $modes) {
      $function = $module .'_ds_api';
      $api_info = $function();
      if ($api_info['views_base'] == $views_base_table) {
        foreach ($modes as $key => $title) {
          $build_mode_options[$key] = $title['title'];
        }
      }
    }

    $form['build_mode'] = array(
      '#type' => 'select',
      '#title' => t('Select the build mode'),
      '#default_value' => $this->options['build_mode'],
      '#options' => $build_mode_options,
      '#description' => t('Make sure you select a build mode which is compatible with the base table of this view which is %base_table. <br />Also note that if you excluded build modes, you will still see them listed here as it\'s possible to list different types in a view.', array('%base_table' => $this->view->base_table))
    );

    $form['advanced'] = array(
      '#type' => 'checkbox',
      '#title' => t('Use the advanced build mode selector'),
      '#description' => t('This gives you the opportunity to have more control in a list, eg the first build mode on the object is A and the rest B.<br /> There is no UI for this, you need to create a function named like this: ds_views_row_adv_VIEWSNAME. All logic is in that function.<br/>Note: the build mode selected above is not passed in that function, only the variables.'),
      '#default_value' => $this->options['advanced'],
    );

  }
}
