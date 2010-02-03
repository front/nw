<?php


/**
 * Implementation of hook_default_view_views().
 */
function aef_easy_view_views_default_views() {

  $view = new view;
  $view->name = 'aef_easy_view_plain';
  $view->description = 'A view that basically display the nodes that are given in parameter. Used for the manual list.';
  $view->tag = '';
  $view->view_php = '';
  $view->base_table = 'node';
  $view->is_cacheable = FALSE;
  $view->api_version = 2;
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->override_option('arguments', array(
    'nid' => array(
      'default_action' => 'ignore',
      'style_plugin' => 'default_summary',
      'style_options' => array(),
      'wildcard' => 'all',
      'wildcard_substitution' => 'All',
      'title' => '',
      'breadcrumb' => '',
      'default_argument_type' => 'fixed',
      'default_argument' => '',
      'validate_type' => 'none',
      'validate_fail' => 'not found',
      'break_phrase' => 1,
      'not' => 0,
      'id' => 'nid',
      'table' => 'node',
      'field' => 'nid',
      'validate_user_argument_type' => 'uid',
      'validate_user_roles' => array(
        '2' => 0,
        '3' => 0,
        '4' => 0,
        '8' => 0,
        '5' => 0,
        '9' => 0,
        '6' => 0,
        '7' => 0,
      ),
      'relationship' => 'none',
      'default_options_div_prefix' => '',
      'default_argument_user' => 0,
      'default_argument_fixed' => '',
      'default_argument_php' => '',
      'validate_argument_node_type' => array(
        'aef_ct_rss_feed' => 0,
        'aef_ct_rss_story' => 0,
        'article' => 0,
        'book' => 0,
        'breakingnews' => 0,
        'category_frontpage' => 0,
        'dossier' => 0,
        'edition' => 0,
        'edition_bak' => 0,
        'element_multimedia' => 0,
        'emission' => 0,
        'emission_bak' => 0,
        'feed' => 0,
        'page' => 0,
        'reportage' => 0,
        'special_report' => 0,
        'special_report_bak' => 0,
        'special_report_frontpage' => 0,
        'special_report_frontpage_bak' => 0,
        'story' => 0,
        'test' => 0,
        'urgent' => 0,
        'urgent_bak' => 0,
        'video' => 0,
        'wire' => 0,
      ),
      'validate_argument_node_access' => 0,
      'validate_argument_nid_type' => 'nid',
      'validate_argument_vocabulary' => array(
        '5' => 0,
        '10' => 0,
        '7' => 0,
        '6' => 0,
        '12' => 0,
        '4' => 0,
        '17' => 0,
        '14' => 0,
      ),
      'validate_argument_type' => 'tid',
      'validate_argument_transform' => 0,
      'validate_user_restrict_roles' => 0,
      'validate_argument_node_flag_name' => '*relationship*',
      'validate_argument_node_flag_test' => 'flaggable',
      'validate_argument_node_flag_id_type' => 'id',
      'validate_argument_user_flag_name' => '*relationship*',
      'validate_argument_user_flag_test' => 'flaggable',
      'validate_argument_user_flag_id_type' => 'id',
      'validate_argument_php' => '',
    ),
  ));
  $handler->override_option('access', array(
    'type' => 'none',
  ));
  $handler->override_option('cache', array(
    'type' => 'none',
  ));
  $handler->override_option('row_plugin', 'node');
  $handler->override_option('row_options', array(
    'relationship' => 'none',
    'build_mode' => 'teaser',
    'links' => 1,
    'comments' => 0,
  ));



  $views[$view->name] = $view;

  return $views;
}
