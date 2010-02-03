<?php

/**
 * Implementation of hook_field_info().
 */
function aef_easy_view_field_info() {
  return array(
    'viewinstancereference' => array(
      'label' => t('View Instance Reference'),
      'description' => t('Store view instance reference in the database.'),
    ),
  );
}

/**
 * Implementation of hook_field_settings().
 */
function aef_easy_view_field_settings($op, $field) {
  switch ($op) {
    case 'form':
      $form = array();
      return $form;

    case 'save':
      return array();

    case 'database columns':
      $columns['viid'] = array('type' => 'int', 'unsigned' => TRUE, 'not null' => FALSE, 'views' => TRUE);
      $columns['nodeformatter'] = array('type' => 'varchar', 'length' => '255', 'not null' => FALSE);
      $columns['numberofnodes'] = array('type' => 'int', 'unsigned' => FALSE, 'not null' => FALSE, 'views' => FALSE);
      $columns['numberofnodesperpage'] = array('type' => 'int', 'unsigned' => FALSE, 'not null' => FALSE, 'views' => FALSE);
      $columns['readmoretext'] = array('type' => 'varchar', 'length' => '255', 'not null' => FALSE);
      $columns['readmorelink'] = array('type' => 'varchar', 'length' => '255', 'not null' => FALSE);
      return $columns;

  }
}

/**
 * Implementation of hook_field().
 */
function aef_easy_view_field($op, &$node, $field, &$items, $teaser, $page) {
  switch ($op) {
    case 'validate':
      return $items;

    case 'sanitize':
  }
}


/**
 * Implementation of hook_content_is_empty().
 */
function aef_easy_view_content_is_empty($item, $field) {
  if (empty($item['viid']) && (string)$item['viid'] !== '0') {
    return TRUE;
  }
  return FALSE;
}

/**
 * Implementation of hook_field_formatter_info().
 */
function aef_easy_view_field_formatter_info() {
  return array(
    'default' => array(
      'label' => t('Default'),
      'field types' => array('viewinstancereference'),
      'multiple values' => CONTENT_HANDLE_CORE,
    ),
  );
}

/**
 * Theme function for 'default' text field formatter.
 */
function theme_aef_easy_view_formatter_default($element) {
	// Inside a View this function may be called with null data.  In that case,
	// just return.
	if (empty($element['#item'])) {
		return '';
	}

	$html = "";

  $html = theme('aef_easy_view_default', $element['#item']['viid'], $element['#item']['nodeformatter'], $element['#item']['numberofnodes'], $element['#item']['numberofnodesperpage'], $element['#item']['readmoretext'], $element['#item']['readmorelink']);


	return $html;
}


/**
 * Implementation of hook_widget_info().
 *
 * Here we indicate that the content module will handle
 * the default value and multiple values for these widgets.
 *
 * Callbacks can be omitted if default handing is used.
 * They're included here just so this module can be used
 * as an example for custom modules that might do things
 * differently.
 */
function aef_easy_view_widget_info() {
  return array(
    'aef_easy_view_creator' => array(
      'label' => t('View instance creator and selector'),
      'field types' => array('viewinstancereference'),
      'multiple values' => CONTENT_HANDLE_CORE,
      'callbacks' => array(
        'default value' => CONTENT_CALLBACK_DEFAULT,
      ),
    ),
  );
}

/**
 * Implementation of FAPI hook_elements().
 *
 * Any FAPI callbacks needed for individual widgets can be declared here,
 * and the element will be passed to those callbacks for processing.
 *
 * Drupal will automatically theme the element using a theme with
 * the same name as the hook_elements key.
 *
 * Autocomplete_path is not used by text_widget but other widgets can use it
 * (see nodereference and userreference).
 */
function aef_easy_view_elements() {
  return array(
    'aef_easy_view_creator' => array(
      '#input' => TRUE,
      '#columns' => array('viid'), 
      '#delta' => 0,
      '#process' => array('aef_easy_view_creator_process'),
      '#element_validate' => array('aef_easy_view_creator_validate'),
      '#value_callback' => 'aef_easy_view_widget_value',
      '#autocomplete_path' => FALSE,
    ),
  );
}

/**
 * Implementation of hook_widget_settings().
 */
function aef_easy_view_widget_settings($op, $widget) {
  switch ($op) {
    case 'form':
      $form = array();
      $form['easy_view_allowed_formatters'] = array(
        '#type' => 'checkboxes',
        '#title' => t('Allowed node formatters'),
        '#default_value' => aef_utilities_check_cck_settings_array($widget['easy_view_allowed_formatters']),
        '#options' => _aef_easy_view_formatter_options(),
        '#description' => t('Select the node formatters that will be proposed to editors to theme the nodes of the view.'),
      );
      $form['easy_view_default_formatter'] = array(
        '#type' => 'select',
        '#title' => t('Default node formatters'),
        '#default_value' => $widget['easy_view_default_formatter'],
        '#options' => _aef_easy_view_formatter_options(),
        '#description' => t('Select the node formatter that will be selected by default.'),
      );
      $paging = array();  
      foreach(array(1,2,3) as $i)
        $paging[$i] = format_plural($i, 'One per page', '@count per page');
      $form['easy_view_default_allowed_pagination'] = array(
        '#type' => 'checkboxes',
        '#title' => t('Allowed pagination'),
        '#default_value' => aef_utilities_check_cck_settings_array($widget['easy_view_default_allowed_pagination']),
        '#options' => $paging,
        '#description' => t('Select the pagination (number of items to show per page) that will be proposed to editors.'),
      );

      $form['easy_view_hide_more_link'] = array(
        '#type' => 'checkbox',
        '#title' => t('Hide "Read more" field'),
        '#default_value' => $widget['easy_view_hide_more_link'],
      );

      $form['easy_view_hide_formatter_selection'] = array(
        '#type' => 'checkbox',
        '#title' => t('Advanced - Hide formatter selection'),
        '#default_value' => $widget['easy_view_hide_formatter_selection'],
        '#description' => t('If you hide the node formatter theme selection, you should not rely on the default output provided by the CCK field. This option is useful if you only want to provide the user a way to select content, but not to select the theme of the output. You will then have to programmatically fetch the output with aef_easy_view_results() and theme it.'),
      );
      return $form;

    case 'validate':
      $nb_selected_formatters = 0;
      $default_formatter_available = false;
      foreach($widget['easy_view_allowed_formatters'] as $formatter_name => $selected)
      {
        if($selected != "0")
        {
          $nb_selected_formatters++;
          if($formatter_name == $widget['easy_view_default_formatter'])
            $default_formatter_available = true;
        }
      }
      if($nb_selected_formatters == 0)
        form_set_error('easy_view_allowed_formatters', t('At least select one node formatter.'));
      if($default_formatter_available == false)
        form_set_error('easy_view_default_formatter', t('The default formatter you choosed is not allowed.'));
      
      return;

    case 'save':
      return array('easy_view_allowed_formatters', 'easy_view_default_formatter', 'easy_view_default_allowed_pagination', 'easy_view_hide_more_link', 'easy_view_hide_formatter_selection');
  }
}

/**
 * Implementation of hook_widget().
 *
 * Attach a single form element to the form. It will be built out and
 * validated in the callback(s) listed in hook_elements. We build it
 * out in the callbacks rather than here in hook_widget so it can be
 * plugged into any module that can provide it with valid
 * $field information.
 *
 * Content module will set the weight, field name and delta values
 * for each form element. This is a change from earlier CCK versions
 * where the widget managed its own multiple values.
 *
 * If there are multiple values for this field, the content module will
 * call this function as many times as needed.
 *
 * @param $form
 *   the entire form array, $form['#node'] holds node information
 * @param $form_state
 *   the form_state, $form_state['values'][$field['field_name']]
 *   holds the field's form values.
 * @param $field
 *   the field array
 * @param $items
 *   array of default values for this field
 * @param $delta
 *   the order of this item in the array of subelements (0, 1, 2, etc)
 *
 * @return
 *   the form item for a single element for this field
 */
function aef_easy_view_widget(&$form, &$form_state, $field, $items, $delta = 0) {
  $element = array(
    '#type' => 'aef_easy_view_creator',
    '#default_value' => isset($items[$delta]) ? $items[$delta] : '',
  );
  return $element;
}

/**
 * Process an individual element.
 *
 * Build the form element. When creating a form using FAPI #process,
 * note that $element['#value'] is already set.
 *
 * The $fields array is in $form['#field_info'][$element['#field_name']].
 */
function aef_easy_view_creator_process($element, $edit, $form_state, $form) {

  $item = $element['#value'];
  $easy_views = aef_easy_view_get_easy_views();
  $field = $form['#field_info'][$element['#field_name']];
  $field_key = $element['#columns'][0];
  $delta = $element['#delta'];
  $selected_easy_view_name = $form['#post'][$element['#field_name']][$delta]['viid_creator']['view_selector'];


  //Add the CSS && JS. Also add the Views CSS in behalf of views.
  drupal_add_css(drupal_get_path('module', 'views') . '/css/views.css');
  drupal_add_js(drupal_get_path('module', 'aef_easy_view') . '/aef_easy_view.js');

  //It will be used, and may not be linked if called in AHAH.
  drupal_add_js('misc/tabledrag.js', 'core');

  //prepare the list of easy views
  $easy_views_options = array('' => t('- Select an automated list -'));
  foreach($easy_views as $easy_view)
  {
    $easy_views_options[$easy_view['key']] = $easy_view['title'];
  }

  //If we already have an easy view referenced, load it
  if(is_numeric($item['viid']) && $item['viid'] > 0)
  {
    $current_easy_view = aef_easy_view_load_record($item['viid']);
  }

  //Determine in which state we are
  //State = 1 : Initial state, only the easy view selection box is here
  //State = 2 : new edit state, we are creating a brand new easy view
  //State = 3 : Saved state, only the name of the easy view is here, with the edit/remove buttons
  //State = 4 : Edit state, we are editing the settings of an existing easy view
  //State = 5 : Edit manual list state, we are editing the result of the easy view
  //State = 6 : Saved state of a manual list, only the name of the easy view is here, with the edit/remove buttons
  //State = 7 : Reorder automatic list results state, we are in a page where we reorder results of an automatic list
  //State = 8 : Saved state of a "semi automatic" list, where the results of the automatic list were re-ordered.
  $element_state = 1;
  if(empty($item['viid']) && $selected_easy_view_name != "")
  {
    $element_state = 2;
  }



  //If a viid is set, show its title, and a "remove" button
  $element['viid_viewer'] = array(
    '#prefix' => '<label>' . check_plain($element['#title']) . ':</label><div class="aef-easy-view-view-infos">',
    '#suffix' => '</div>',
  );

  if(is_array($current_easy_view['data']['manual_list_items']))
    $type_text = '<span class="aef_easy_view_viid_viewer_viid_title_type_man">(' . t("Manual static list") . ')</span>';
  else if(is_array($current_easy_view['data']['semi_automatic_list_items']))
    $type_text = '<span class="aef_easy_view_viid_viewer_viid_title_type_semauto">(' . t("Re-ordered automatically updated list") . ')</span>';
  else
    $type_text =  '<span class="aef_easy_view_viid_viewer_viid_title_type_auto">(' .  t("Automatically updated list") . ')</span>';

  $element['viid_viewer']['title'] = array(
    '#value' => '<div class="aef_easy_view_viid_viewer_viid_title">' . 
      check_plain($current_easy_view['title']) . 
      ' ' . 
      $type_text .
      '</div>',
  );
  //To fill later
  $element['viid_viewer']['results'] = array();
  $element['viid_viewer']['edit'] = array(
    '#name' => $element['#field_name'] .'_'. $element['#delta'] .'_edit',
    '#type' => 'submit',
    '#value' => t('Edit this automated list'),
    '#ahah' => array(
        "path" => "aef_easy_view/ahah/" . $element['#type_name'] . '/' . $element['#field_name'] . '/' . $delta,
        "effect" => "fade",
        "wrapper" => $element['#id'] . '-wrapper', 
    ),
    '#field_name' => $element['#field_name'],
    '#post' => $element['#post'],
    '#attributes' => array('class' => 'aef_easy_view_viid_viewer_viid_edit')
  );
  $element['viid_viewer']['reorder'] = array(
    '#name' => $element['#field_name'] .'_'. $element['#delta'] .'_reorder',
    '#type' => 'submit',
    '#value' => t('Reorder the results'),
    '#ahah' => array(
        "path" => "aef_easy_view/ahah/" . $element['#type_name'] . '/' . $element['#field_name'] . '/' . $delta,
        "effect" => "fade",
        "wrapper" => $element['#id'] . '-wrapper', 
    ),
    '#field_name' => $element['#field_name'],
    '#post' => $element['#post'],
    '#attributes' => array('class' => 'aef_easy_view_viid_viewer_viid_reorder')
  );
  $element['viid_viewer']['edit_manual_list'] = array(
    '#name' => $element['#field_name'] .'_'. $element['#delta'] .'_edit_manual_list',
    '#type' => 'submit',
    '#value' => t('Edit this manual list'),
    '#ahah' => array(
        "path" => "aef_easy_view/ahah/" . $element['#type_name'] . '/' . $element['#field_name'] . '/' . $delta,
        "effect" => "fade",
        "wrapper" => $element['#id'] . '-wrapper', 
    ),
    '#field_name' => $element['#field_name'],
    '#post' => $element['#post'],
    '#attributes' => array('class' => 'aef_easy_view_viid_viewer_viid_edit_manual')
  );
  $element['viid_viewer']['convert'] = array(
    '#name' => $element['#field_name'] .'_'. $element['#delta'] .'_convert',
    '#type' => 'submit',
    '#value' => t('Convert into a manual list'),
    '#ahah' => array(
        "path" => "aef_easy_view/ahah/" . $element['#type_name'] . '/' . $element['#field_name'] . '/' . $delta,
        "effect" => "fade",
        "wrapper" => $element['#id'] . '-wrapper', 
    ),
    '#field_name' => $element['#field_name'],
    '#post' => $element['#post'],
    '#attributes' => array('class' => 'aef_easy_view_viid_viewer_viid_convert')
  );
  $element['viid_viewer']['convert_back'] = array(
    '#type' => 'submit',
    '#value' => t('Go back to the default automatic list'),
    '#ahah' => array(
        "path" => "aef_easy_view/ahah/" . $element['#type_name'] . '/' . $element['#field_name'] . '/' . $delta,
        "effect" => "fade",
        "wrapper" => $element['#id'] . '-wrapper', 
    ),
    '#field_name' => $element['#field_name'],
    '#post' => $element['#post'],
    '#attributes' => array('class' => 'aef_easy_view_viid_viewer_viid_convert_back')
  );
  $element['viid_viewer']['remove'] = array(
    '#name' => $element['#field_name'] .'_'. $element['#delta'] .'_remove',
    '#type' => 'submit',
    '#value' => t('Remove'),
    '#ahah' => array(
        "path" => "aef_easy_view/ahah/" . $element['#type_name'] . '/' . $element['#field_name'] . '/' . $delta,
        "effect" => "fade",
        "wrapper" => $element['#id'] . '-wrapper', 
    ),
    '#field_name' => $element['#field_name'],
    '#post' => $element['#post'],
    '#attributes' => array('class' => 'aef_easy_view_viid_viewer_viid_remove')
  );


  // Because the output of this field changes depending on the button clicked,
  // we need to ask FAPI immediately if the remove button was clicked.
  // It's not good that we call this private function, but
  // $form_state['clicked_button'] is only available after this #process
  // callback is finished.
  if (_form_button_was_clicked($element['viid_viewer']['remove'])) {
    //Remove the view instance
    $item = array('viid' => 0);
  }
  else if (_form_button_was_clicked($element['viid_viewer']['edit'])) {
    //Get the saved form data from the view instance, put them as value of the provided element,
    //and remove it
    $item['viid'] = 0;
    $selected_easy_view_name = $current_easy_view['easy_view_name'];
    $element_state = 4;
  }
  else if(_form_button_was_clicked($element['viid_viewer']['convert']))
  {
    //Execute the view, retrieve the results and put them in the easy view
    $element_state = 5;

    $results = aef_easy_view_auto_results($current_easy_view, 20);
    foreach($results as $result)
    {
      $current_easy_view['data']['manual_list_items'][] = $result->nid;
    }
  }
  else if(_form_button_was_clicked($element['viid_viewer']['edit_manual_list']))
  {
    $element_state = 5;
  }
  else if(_form_button_was_clicked($element['viid_viewer']['reorder']))
  {
    $element_state = 7;

    if(isset($current_easy_view['data']['semi_automatic_list_items']['items']) == false ||
      count($current_easy_view['data']['semi_automatic_list_items']['items']) == 0)
    {
      //Semi automatic list is created
      $results = aef_easy_view_auto_results($current_easy_view);
      foreach($results as $result)
      {
        $current_easy_view['data']['semi_automatic_list_items']['items'][] = array(
          'nid' => $result->nid,
          'fixed' => false,
        );
      }
    }
    else
    {
      //Semi automatic list already created, we are updating the list.
      $current_easy_view['data']['semi_automatic_list_items']['items'] = 
        _aef_easy_view_semi_automatic_results($current_easy_view);
    }
  }
  else if(!empty($item['viid']) && !_form_button_was_clicked($element['viid_viewer']['edit']))
  {
    if(is_array($current_easy_view['data']['manual_list_items']))
      //Saved manual list
      $element_state = 6;
    else if(is_array($current_easy_view['data']['semi_automatic_list_items']['items']))
      //Saved semi automatic view
      $element_state = 8;
    else
      //Saved automatic view
      $element_state = 3;
  }


  //Put a div wrapper here
  $element['#prefix'] = '<div id="' . $element['#id'] . '-wrapper" class="aef_easy_view_wrapper">';
  $element['#suffix'] = '</div>';



  //
  // At this points, state info is ready.
  //


  //
  //Viewer area
  //

  //List the results
  $results = array();
  $results_nodetitle = array();
  if($element_state == 3)
  {
    $results = aef_easy_view_auto_results($current_easy_view, 10);
  }
  else if($element_state == 8)
  {
    foreach($current_easy_view['data']['semi_automatic_list_items']['items'] as $node_infos)
    {
      $node = null;
      $node->nid = $node_infos['nid'];
      $results[] = $node;
    }
  }
  else if($element_state == 6)
  {
    foreach($current_easy_view['data']['manual_list_items'] as $nid)
    {
      $node = null;
      $node->nid = $nid;
      $results[] = $node;
    }
  }
  foreach($results as $node)
  {
    $node = node_load($node->nid);
    $results_nodetitle[] = '<li>' . l($node->title, 'node/' . $node->nid, array('attributes' => array('class' => 'aef_embedded_edit'))) . '</li>';
  }
  if(count($results) >= 10 && $element_state == 3)
  {
    $results_nodetitle[] = '<li>...</li>';
  }
  $element['viid_viewer']['results'] = array(
    '#value' => '<ul>' . implode('', $results_nodetitle) . '</ul>',
  );

  //
  //View creator
  //

  //Wrapper
  $element['viid_creator'] = array(
  );

  //View selector
  $element['viid_creator']['view_selector'] = array(
    '#type' => 'select',
    '#options' => $easy_views_options,
    '#value' => $selected_easy_view_name,
    '#attributes' => array('class' => 'aef_easy_view_creator'),
    '#ahah' => array(
        "path" => "aef_easy_view/ahah/" . $element['#type_name'] . '/' . $element['#field_name'] . '/' . $delta,
        "effect" => "fade",
        "wrapper" => $element['#id'] .'-wrapper',
        "event" => "change",
    ),

    // The following values were set by the content module and need
    // to be passed down to the nested element.
    '#title' => $element['#title'],
//    '#description' => $element['#description'],
    '#required' => $element['#required'],
    '#field_name' => $element['#field_name'],
    '#type_name' => $element['#type_name'],
    '#delta' => $element['#delta'],
    '#columns' => $element['#columns'],
  );


  //Settings form area
  $form_settings = "";
  $element['viid_creator']['view_settings'] = array(
    '#prefix' => '<div id="' . $element['#id'] . '-view-settings" class="aef-easy-view-view-settings">',
    '#suffix' => '</div>',
  );

  //Show the easy view edit form
  if($element_state == 2 || $element_state == 4)
  {
    $selected_easy_view = $easy_views[$selected_easy_view_name];

    //Add a few items before adding this setting form to the main form
    $title = array(
      '#type' => 'textfield',
      '#title' => t('Internal Name'),
      '#weight' => -0.01,
      '#value' => ($current_easy_view != null) ? $current_easy_view['data']['form_data']['title'] : $edit['viid_creator']['view_settings']['form']['title'],
      );
    $submit = array(
      '#type' => 'submit',
      '#value' => ($element_state == 2) ? t('Create the automated list') : t('Update the automated list'),
      '#ahah' => array(
          "path" => "aef_easy_view/ahah/" . $element['#type_name'] . '/' . $element['#field_name'] . '/' . $delta,
          "effect" => "fade",
          "wrapper" => $element['#id'] . '-wrapper', 
      ),
    );
    $easy_view_settings_element = array(
      '#type' => $selected_easy_view['element_name'],
      '#field_name' => $element['#field_name'],
      '#field_name_wrapper' => $element['#id'] .'-wrapper',
      '#delta' => $element['#delta'],
      '#type_name' => $element['#type_name'],
      '#value' => ($current_easy_view != null) ? $current_easy_view['data']['form_data'] : $edit['viid_creator']['view_settings']['form'],
    );

    $element['viid_creator']['view_settings']['form']['title'] = $title;
    $element['viid_creator']['view_settings']['form']['element'] = $easy_view_settings_element;
    $element['viid_creator']['view_settings']['form']['submit'] = $submit;
  }

  //Show the manual list edit form
  if($element_state == 5)
  {
    $selected_easy_view = $easy_views[$selected_easy_view_name];

    $element['viid_creator']['view_settings']['form']['manual_list'] = array(
      '#theme' => 'aef_easy_view_results',
      '#id' => $element['#id'] .'-results',
      );
    for($i = 0; $i < 20; $i++)
    {
      $nid_to_display = $current_easy_view['data']['manual_list_items'][$i];
      $value = ($node_to_display = node_load($nid_to_display))?$node_to_display->title . " [nid:" . $nid_to_display . ']':"";
      $element['viid_creator']['view_settings']['form']['manual_list'][$i] = array(
        'nid' => array(
          '#type' => 'textfield',
          '#value' => $value,
          '#autocomplete_path' => 'aef_easy_view/noderef_autocomplete',
          '#attributes' => array("class" => 'aef-nodeselect-droppable-blockdisplay'),
        ),
        '_weight' => array(
          '#type' => 'select',
          '#attributes' => array('class' => $element['#id'] .'-results-delta-order'),
          '#options' => range(-10,10),
          '#value' => $i,
        )
      );
    }


//theme('table', $header, $rows, array('id' => 'my-module-table'));

   $element['viid_creator']['view_settings']['form']['manual_list_save'] = array(
    '#type' => 'submit',
    '#value' => t('Save the manual list'),
    '#ahah' => array(
      "path" => "aef_easy_view/ahah/" . $element['#type_name'] . '/' . $element['#field_name'] . '/' . $delta,
      "effect" => "fade",
      "wrapper" => $element['#id'] . '-wrapper', 
      ),
    );
  }

  //Show the reordering edit form
  if($element_state == 7)
  {
    $element['viid_creator']['view_settings']['form']['results_to_reorder'] = array(
      '#theme' => 'aef_easy_view_results',
      '#id' => $element['#id'] .'-results',
      );
    $i = 0;
    foreach($current_easy_view['data']['semi_automatic_list_items']['items'] as $item_infos)
    {
      $node = node_load($item_infos['nid']);
      $noderef_value = $node->title . " [nid:" . $item_infos['nid'] . ']';
      $element['viid_creator']['view_settings']['form']['results_to_reorder'][] = array(
        'nid' => array(
          '#type' => 'textfield',
          '#value' => $noderef_value,
          '#autocomplete_path' => 'aef_easy_view/noderef_autocomplete',
          '#attributes' => array("class" => 'aef-nodeselect-droppable-blockdisplay'),
        ),
        'fixed' => array(
          '#type' => 'checkbox',
          '#title' => t("Stick"),
          '#value' => $item_infos['fixed'],
        ),
        '_weight' => array(
          '#type' => 'select',
          '#attributes' => array('class' => $element['#id'] .'-results-delta-order'),
          '#options' => range(-10,10),
          '#value' => $i,
        )
      );
      $i++;
    }

   $element['viid_creator']['view_settings']['form']['results_to_reorder_save'] = array(
    '#type' => 'submit',
    '#value' => t('Save the re-ordered automatic list'),
    '#ahah' => array(
      "path" => "aef_easy_view/ahah/" . $element['#type_name'] . '/' . $element['#field_name'] . '/' . $delta,
      "effect" => "fade",
      "wrapper" => $element['#id'] . '-wrapper', 
      ),
    );
  }

  // Used so that hook_field('validate') knows where to flag an error.
  $element['_error_element'] = array(
    '#type' => 'value',
    '#value' => implode('][', array_merge($element['#parents'], array('viid_creator'))),
  );


  // Set the viid.
  $element['viid'] = array(
    '#type' => 'hidden',
    '#value' => $item['viid'],
  );


  //
  // Display area
  //

  //Number of items to show
  $number_of_nodes = array();
  foreach(range(1,20) as $i)
    $number_of_nodes[$i] = format_plural($i, '1 item', '@count items');
  $element['numberofnodes'] = array(
    '#title' => t('Number of items to display'),
    '#type' => 'select',
    '#value' => (($item['numberofnodes'] != 0)? $item['numberofnodes'] : 10),
    '#options' => $number_of_nodes,
    '#prefix' => '<div class="container-inline">',
    '#suffix' => '</div>',
  );

  //Paging of items
  $allowed_pagination = aef_utilities_check_cck_settings_array(
    $field['widget']['easy_view_default_allowed_pagination']);
  if(is_array($field['widget']['easy_view_default_allowed_pagination']) == false)
    $allowed_pagination = array(1,2,3);

  $paging = array(0 => t('All in the same page'));
  foreach($allowed_pagination as $i)
    $paging[$i] = format_plural($i, 'One per page', '@count per page');
  $element_attributes = array(
      "class" => 'aef-easy-view-pagination',
  );
  //Fetching the carousel <-> formatter links, so that we can hide formatters unsuitable for a given carousel
  $formatter_carousel_links = aef_jcarousel_get_formatter_carousel_links();
  $pagination_theme = array("all", "aef_easy_view_one_item", "aef_easy_view_two_items", "aef_easy_view_default");
  $pagination_ids = array(0,1,2,3);
  $i = 0;
  foreach($pagination_theme as $pagination)
  {
    if(isset($formatter_carousel_links[$pagination]))
    {
      $element_attributes["pagination-" . $pagination_ids[$i] . "-formatters-default"] =
        $formatter_carousel_links[$pagination]['default'];
      if($formatter_carousel_links[$pagination]['default'] == "hide")
        $element_attributes["pagination-" . $pagination_ids[$i] . "-formatters"] = 
          implode(" ", $formatter_carousel_links[$pagination]['formatters']['show']);
      else
      {
        $formatters_to_hide = $formatter_carousel_links[$pagination]['formatters']['hide'];
        $formatted_formatters_to_hide = array();
        foreach($formatters_to_hide as $formatter_to_hide)
        {
          $formatted_formatters_to_hide[] = "!" . $formatter_to_hide;
        }
        $element_attributes["pagination-" . $pagination_ids[$i] . "-formatters"] = 
          implode(" ", $formatted_formatters_to_hide);
      }
    }
    $i++;
  }
  $element['numberofnodesperpage'] = array(
    '#title' => t('Item pagination'),
    '#type' => ((count($allowed_pagination))?'select':'hidden'),
    '#value' => $item['numberofnodesperpage'],
    '#options' => $paging,
    '#prefix' => '<div class="container-inline">',
    '#suffix' => '</div>',
    '#attributes' => $element_attributes,
  );

  //The possible node formatters to use
  $allowed_formatters = array();
  $allowed_formatters_settings = aef_utilities_check_cck_settings_array($field['widget']['easy_view_allowed_formatters']);
  foreach(_aef_easy_view_formatter_options() as $formatter_name => $formatter_title)
  {
    if($allowed_formatters_settings[$formatter_name] == $formatter_name)
      $allowed_formatters[$formatter_name] = $formatter_title;
  }
  $element['nodeformatter'] = array(
    '#title' => t('Item theme'),
    '#type' => (($field['widget']['easy_view_hide_formatter_selection'])?'hidden':'select'),
    '#value' => ($item['nodeformatter'] != null) ? $item['nodeformatter'] : $field['widget']['easy_view_default_formatter'],
    '#options' => $allowed_formatters,
    '#prefix' => '<div class="container-inline">',
    '#suffix' => '</div>',
    '#attributes' => array(
      'class' => 'aef-easy-view-nodeformatter'
    ),
  );

  $element['readmoretext'] = array(
    '#title' => t('"Read more" text'),
    '#type' => (($field['widget']['easy_view_hide_more_link'])?'hidden':'textfield'),
    '#value' => $item['readmoretext'],
    '#prefix' => '<div class="container-inline">',
    '#size' => '10',
  );

  $element['readmorelink'] = array(
    '#title' => t('link'),
    '#type' => (($field['widget']['easy_view_hide_more_link'])?'hidden':'textfield'),
    '#value' => $item['readmorelink'],
    '#size' => '20',
    '#suffix' => '</div>',
  );

  //The preview button
  if($field['widget']['easy_view_hide_formatter_selection'] == false)
  {
    $element['nodeformatter_preview'] = array(
      '#value' => t('Preview'),
      '#type' => 'submit',
      '#suffix' => '<div id="'. $element['#field_name'] .'-preview"></div><div class="clear-both"></div>',
      '#ahah' => array(
        "path" => "aef_easy_view/preview/" . $element['#field_name'],
        "wrapper" => $element['#field_name'] .'-preview',
        "effect" => "fade",
        ),
    );
  }

  //Depending of the state of the element, show/hide some parts
  $element['viid_viewer']['#access'] = ($element_state == 3 || $element_state == 6 || $element_state == 8);
  $element['viid_viewer']['edit']['#access'] = ($element_state == 3);
  $element['viid_viewer']['reorder']['#access'] = ($element_state == 3 || $element_state == 8);
  $element['viid_viewer']['convert']['#access'] = ($element_state == 3);
  $element['viid_viewer']['edit_manual_list']['#access'] = ($element_state == 6);
  $element['viid_viewer']['convert_back']['#access'] = ($element_state == 6 || $element_state == 8);
  $element['viid_creator']['#access'] = ($element_state == 1 || $element_state == 2 || $element_state == 4 || $element_state == 5 || $element_state == 7);
  $element['viid_creator']['view_selector']['#access'] = ($element_state == 1 || $element_state == 2 || $element_state == 4);
  $element['numberofnodes']['#access'] = ($element_state != 1);
  $element['nodeformatter']['#access'] = ($element_state != 1);
  $element['numberofnodesperpage']['#access'] = ($element_state != 1);
  $element['readmoretext']['#access'] = ($element_state != 1);
  $element['readmorelink']['#access'] = ($element_state != 1);
  $element['nodeformatter_preview']['#access'] = ($element_state == 2 || $element_state == 3 || $element_state == 4 || $element_state == 6 || $element_state == 8);


  //Add a validation callback
  //$element['#element_validate'] = array('aef_easy_view_creator_process_validate');

  return $element;
}

//function aef_easy_view_creator_process_validate($element, &$form_state)
//{
//}


/**
 * FAPI theme for an individual text elements.
 *
 * $element['#field_name'] contains the field name
 * $element['#delta]  is the position of this element in the group
 */
function theme_aef_easy_view_creator($element) {
  return $element['#children'];
}


/**
 * The #value_callback for the aef_easy_view_creator type element.
 */
function aef_easy_view_widget_value($element, $edit = FALSE) {
  global $user;

  //If the Create/update button was clicked, or if the main "Save" button of the node edit form was clicked
  if($edit && 
      (
        $element['#post']['op'] == t("Create the automated list") || 
        $element['#post']['op'] == t('Update the automated list') || 
        ($element['#post']['op'] == t("Save")) && !is_array($edit['viid_creator']['view_settings']['form']['manual_list']) 
      )
    )
  {
    $easy_views = aef_easy_view_get_easy_views();

    //If a view settings form is present, create a Parametized view Instance.
    if(is_array($edit['viid_creator']['view_settings']['form']))
    {
      //First validate the input, from our element
      $form_state = array('values' => $element['#post']);
      aef_easy_view_creator_validate($element, $form_state);
      //Then from the provided custom element
      $used_element = _element_info($easy_views[$edit['viid_creator']['view_selector']]['element_name']);
      $element_validate_callback = $used_element['#element_validate'];
      if(count($element_validate_callback) > 0)
      {
        foreach($element_validate_callback as $callback)
        {
          $callback($element, $form_state);
        }
      }

      //Then call the submit functions
      if(count(form_get_errors()) == 0)
      {
        $form_submit_callback = $easy_views[$edit['viid_creator']['view_selector']]['form_submit'];
        if($form_submit_callback != "")
        {
          //Call the function that will return us the parameters for the view
          $result = $form_submit_callback($edit['viid_creator']['view_settings']['form']);

          //Save this as a new entry
          $record['easy_view_name'] = $edit['viid_creator']['view_selector'];
          $record['title'] = $edit['viid_creator']['view_settings']['form']['title'];
          $record['data'] = array('view_params' => $result['view_params'], 'feed_view_params' => $result['feed_view_params'], 'form_data' => $result['form_data']);
          $record['uid'] = $user->uid;
          $record['created'] = $record['changed'] = time();
          drupal_write_record('aef_easy_view_views', $record);

          $item = array('viid' => $record['viid'], 'nodeformatter' => $edit['nodeformatter'], 'numberofnodes' => $edit['numberofnodes'], 'numberofnodesperpage' => $edit['numberofnodesperpage']);
        }
      }
    }
  }
  //If the Save manual list button was clicked, or if the main "Save" button of the node edit form was clicked
  else if($edit && 
      (
        $element['#post']['op'] == t('Save the manual list') || 
        ($element['#post']['op'] == t("Save") && is_array($edit['viid_creator']['view_settings']['form']['manual_list']))
      )
    )
  {
    //$easy_views = aef_easy_view_get_easy_views();

    //If a view settings form is present, create a Parametized view Instance.
    if(is_array($edit['viid_creator']['view_settings']['form']))
    {
      $record = aef_easy_view_load_record($edit['viid']);

      $manual_list_items = array();
      if(is_array($edit['viid_creator']['view_settings']['form']['manual_list']))
      {
        usort($edit['viid_creator']['view_settings']['form']['manual_list'], '_content_sort_items_helper');
        foreach($edit['viid_creator']['view_settings']['form']['manual_list'] as $item)
        {
          preg_match('/^(?:\s*|(.*) )?\[\s*nid\s*:\s*(\d+)\s*\]$/', $item['nid'], $matches);
          list(, $title, $nid) = $matches;
          if(is_numeric($nid) && $nid > 0)
          {
            $manual_list_items[] = $nid;
          }
        }
      }
      $record['data'] = array('manual_list_items' => $manual_list_items) + $record['data'];
      drupal_write_record('aef_easy_view_views', $record);

      $item = array('viid' => $record['viid'], 'nodeformatter' => $edit['nodeformatter'], 'numberofnodes' => $edit['numberofnodes'], 'numberofnodesperpage' => $edit['numberofnodesperpage']);
    }
  }
  else if($edit && $element['#post']['op'] == t('Save the re-ordered automatic list'))
  {
      $record = aef_easy_view_load_record($edit['viid']);
      $semauto_data = array();
      $semi_automatic_list_items = array();
      $items = $edit['viid_creator']['view_settings']['form']['results_to_reorder'];

      if(isset($record['data']['semi_automatic_list_items']) == false)
      {
        //First time it is saved, saved the first nid
        $res = aef_easy_view_auto_results($record, 1);
        $semauto_data['last_fetched_nid'] = $res[0]->nid;
      }
      else
        //Take the one saved in the easy view instance
        $semauto_data['last_fetched_nid'] = $record['data']['semi_automatic_list_items']['last_fetched_nid'];

      if(is_array($items))
      {
        usort($items, '_content_sort_items_helper');
        foreach($items as $item)
        {
          preg_match('/^(?:\s*|(.*) )?\[\s*nid\s*:\s*(\d+)\s*\]$/', $item['nid'], $matches);
          list(, $title, $nid) = $matches;
          if($nid > 0)
          {
            $semi_automatic_list_items[] = array('nid' => $nid, 'fixed' => $item['fixed']);
          }
        }
      }
      $semauto_data += array('items' => $semi_automatic_list_items);
        
      $record['data'] = array('semi_automatic_list_items' => $semauto_data) + $record['data'];
      drupal_write_record('aef_easy_view_views', $record);

      $item = array(
        'viid' => $record['viid'], 
        'nodeformatter' => $edit['nodeformatter'], 
        'numberofnodes' => $edit['numberofnodes'], 
        'numberofnodesperpage' => $edit['numberofnodesperpage']
      );
  }
  else if($edit && $element['#post']['op'] == t('Go back to the default automatic list'))
  {
    $record = aef_easy_view_load_record($edit['viid']);
    unset($record['data']['manual_list_items']);
    unset($record['data']['semi_automatic_list_items']);
    drupal_write_record('aef_easy_view_views', $record);
    $item = array('viid' => $record['viid'], 'nodeformatter' => $edit['nodeformatter'], 'numberofnodes' => $edit['numberofnodes'], 'numberofnodesperpage' => $edit['numberofnodesperpage']);
  }
  else if(!$edit){
    $item = $element['#default_value'];
  }

  return $item;
}

/**
 * AEF Easy view creator validation function.
 */
function aef_easy_view_creator_validate(&$element, &$form_state) 
{
  //$easy_views = aef_easy_view_get_easy_views();
  //$selected_view = $element_form_values['viid_creator']['view_selector'];
  $element_form_values = $form_state['values'][$element['#field_name']][$element['#delta']];

  //If the easy view creator part is shown, and no title is entered, ask for one.
  if(isset($element_form_values['viid_creator']['view_settings']['form']['title']) && $element['viid']['#value'] == 0)
  {
    $easy_view_title = $element_form_values['viid_creator']['view_settings']['form']['title'];
    if($easy_view_title == "")
    {
      form_error($element, t('The internal name of %field is empty.', array('%field' => $element['#title'])));
    }
  }
}


/**
 * Preview function 
 */
function aef_easy_view_preview($field_id)
{
  $easy_views = aef_easy_view_get_easy_views();

  // Lots of code here comes from content_add_more_js
  if (empty($_POST['form_build_id'])) {
    // Invalid request.
    drupal_json(array('data' => ''));
    print "invalid request";
    exit;
  }
  // Retrieve the cached form.
  $form_state = array('submitted' => FALSE);
  $form_build_id = $_POST['form_build_id'];
  $form = form_get_cache($form_build_id, $form_state);
  if (!$form) {
    // Invalid form_build_id.
    drupal_json(array('data' => ''));
    print "invalid form_build_id";
    exit;
  }


  $form_state = array('submitted' => FALSE);
  $form += array(
    '#post' => $_POST,
    '#programmed' => FALSE,
  );
  $form = form_builder($_POST['form_id'], $form, $form_state);
  drupal_validate_form($_POST['form_id'], $form, $form_state);

  //Easy view already saved. Easy
  if($form_state['values'][$field_id][0]['viid'] > 0)
  {
    $preview = theme('status_messages') . 
      content_format($field_id, $form_state['values'][$field_id][0], 'default', $node);
  }
  else
  {
    //Given the form values, compute and display the view that would be displayed
    $easy_view_form_values = $form_state['values'][$field_id][0];

    if($easy_view_form_values['viid_creator']['view_selector'] != "")
    {
      $selected_view_name = $easy_view_form_values['viid_creator']['view_selector'];
      $selected_view = $easy_views[$selected_view_name];
      $form_submit_callback = $selected_view['form_submit'];
      if($form_submit_callback != "")
      {
        //Call the function that will return us the parameters for the view
        $result = $form_submit_callback($easy_view_form_values['viid_creator']['view_settings']['form']);

        $preview = theme('status_messages') . 
          aef_easy_view_display_view($selected_view, $result['view_params'], $easy_view_form_values['nodeformatter'], $easy_view_form_values['numberofnodes'], $easy_view_form_values['numberofnodesperpage']);

      }
    }

  }

  if (trim($preview) != "") {
    $preview = '<div class="aef-easy-view-preview preview"><div class="aef-easy-view-close"><a href="#">' .t('Close'). '</a></div>'. print_r($preview, true) .'</div>';
  }

  //Update the Drupal.settings (for the AHAH behaviors)
  $javascript = drupal_add_js(NULL, NULL);
  $output_js = isset($javascript['setting']) ? '<script type="text/javascript">jQuery.extend(Drupal.settings, '. drupal_to_js(call_user_func_array('array_merge_recursive', $javascript['setting'])) .');</script>' : '';


  $output = $output_js . $preview;
  print drupal_to_js(array('status' => TRUE, 'data' => print_r($output, TRUE)));
  exit;
}

/**
 * Theme a list of draggable entries.
 */
function theme_aef_easy_view_results($element) {
  $id = $element['#id'] . '-results';
  $order_class = $element['#id'] . '-delta-order';

  $header = array(
    array(
      'data' => '',
      'colspan' => 3
    ),
    '',
  );
  $rows = array();

  // Sort items according to '_weight' (needed when the form comes back after
  // preview or failed validation)
  $items = array();
  foreach (element_children($element) as $key) {
    $items[] = &$element[$key];
  }
  //usort($items, '_content_sort_items_value_helper');

  // Add the items as table rows.
  foreach ($items as $key => $item) {
    $item['_weight']['#attributes']['class'] = $order_class;
    $delta_element = drupal_render($item['_weight']);
    $fixed_element = drupal_render($item['fixed']);
    $cells = array(
      array('data' => '', 'class' => 'content-multiple-drag'),
      drupal_render($item),
      array('data' => $fixed_element, 'class' => 'fixed_checkbox'),
      array('data' => $delta_element, 'class' => 'delta-order'),
    );
    $rows[] = array(
      'data' => $cells,
      'class' => 'draggable',
    );
  }

  $output .= theme('table', $header, $rows, array('id' => $id, 'class' => 'aef-easy-view-results-table'));

  drupal_add_tabledrag($id, 'order', 'sibling', $order_class);


  return $output;
}

/**
 * Menu callback; Retrieve a pipe delimited string of autocomplete suggestions for existing users
 */
function aef_easy_view_noderef_autocomplete($string = '') {
  $where = array();
  $args = array();
  $limit = 10;

  if ($string !== '') {
    $where[] = 'n.title '. "LIKE '%%%s%%'";
    $args[] = $string;
  }

  $where_clause = $where ? 'WHERE ('. implode(') AND (', $where) .')' : '';
  $sql = db_rewrite_sql("SELECT n.nid, n.title AS node_title, n.type AS node_type FROM {node} n $where_clause ORDER BY n.title, n.type");
  $result = $limit ? db_query_range($sql, $args, 0, $limit) : db_query($sql, $args);
  $references = array();
  while ($node = db_fetch_object($result)) {
    $references[$node->nid] = array(
      'title' => $node->node_title,
      'rendered' => check_plain($node->node_title),
    );
  }

  $matches = array();
  foreach ($references as $id => $row) {
    // Add a class wrapper for a few required CSS overrides.
    $matches[$row['title'] ." [nid:$id]"] = '<div class="reference-autocomplete">'. $row['rendered'] . '</div>';
  }
  drupal_json($matches);
}
