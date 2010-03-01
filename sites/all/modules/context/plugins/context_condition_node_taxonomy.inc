<?php
// $Id: context_condition_node_taxonomy.inc,v 1.1.2.1 2010/02/09 17:05:22 yhahn Exp $

/**
 * Expose node taxonomy terms as a context condition.
 */
class context_condition_node_taxonomy extends context_condition_node {
  function condition_values() {
    $values = array();
    if (module_exists('taxonomy')) {
      foreach (taxonomy_get_vocabularies() as $vocab) {
        if (empty($vocab->tags)) {
          foreach (taxonomy_get_tree($vocab->vid) as $term) {
            $values[$term->tid] = $term->name;
          }
        }
      }
    }
    return $values;
  }

  function condition_form($context) {
    $form = parent::condition_form($context);
    $form['#type'] = 'select';
    $form['#size'] = 12;
    $form['#multiple'] = TRUE;
    $form['#options'] = taxonomy_form_all();
    return $form;
  }

  function execute($node, $op) {
    $map = context_condition_map();
    if (!empty($map['node_taxonomy']) && !empty($node->taxonomy)) {
      foreach ($node->taxonomy as $term) {
        foreach ($this->get_contexts($term->tid) as $context) {
          // Check the node form option.
          if ($op === 'form') {
            $options = $this->fetch_from_context($context, 'options');
            if (!empty($options['node_form'])) {
              $this->condition_met($context, $term->tid);
            }
          }
          else {
            $this->condition_met($context, $term->tid);
          }
        }
      }
    }
  }
}