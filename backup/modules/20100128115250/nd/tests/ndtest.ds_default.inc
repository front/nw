<?php
// $Id: ndtest.ds_default.inc,v 1.1.2.4 2010/01/27 10:45:57 swentel Exp $

/**
 * @file
 * Display suite default settings.
*/

function _ndtest_ds_default_settings() {
  $data = array(
    'nd' => array(
      'page' => array(
        'teaser' => array(
          'fields' => array(
            'title' => array(
              'weight' => '-19',
              'format' => 'nd_title_h2_link',
              'region' => 'header',
              'label_value' => 'Title',
              'labelformat' => 'hidden',
            ),
            'author' => array(
              'weight' => '-18',
              'format' => 'ds_author_nolink',
              'region' => 'disabled',
              'label_value' => 'Author',
              'labelformat' => 'hidden',
            ),
            'links' => array(
              'weight' => '-17',
              'format' => '',
              'region' => 'disabled',
              'label_value' => 'Links',
              'labelformat' => 'hidden',
            ),
            'read_more' => array(
              'weight' => '-18',
              'format' => '',
              'region' => 'footer',
              'label_value' => 'Read more',
              'labelformat' => 'hidden',
            ),
            'post_date' => array(
              'weight' => '-19',
              'format' => '',
              'region' => 'disabled',
              'label_value' => 'Post date',
              'labelformat' => 'hidden',
            ),
            'body' => array(
              'weight' => '-20',
              'format' => 'nd_bodyfield',
              'region' => 'middle',
              'label_value' => 'Core body',
              'labelformat' => 'hidden',
            ),
            'terms' => array(
              'weight' => '-20',
              'format' => '',
              'region' => 'disabled',
              'label_value' => 'Taxonomy',
              'labelformat' => 'hidden',
            ),
          ),
          'status' => 1,
        ),
        'full' => array(
          'fields' => array(
            'title' => array(
              'weight' => '-16',
              'format' => 'nd_title_h1_nolink',
              'region' => 'disabled',
              'label_value' => 'Title',
              'labelformat' => 'hidden',
            ),
            'author' => array(
              'weight' => '-20',
              'format' => 'ds_author_nolink',
              'region' => 'header',
              'label_value' => 'Author',
              'labelformat' => 'hidden',
            ),
            'links' => array(
              'weight' => '-17',
              'format' => '',
              'region' => 'disabled',
              'label_value' => 'Links',
              'labelformat' => 'hidden',
            ),
            'read_more' => array(
              'weight' => '-18',
              'format' => '',
              'region' => 'disabled',
              'label_value' => 'Read more',
              'labelformat' => 'hidden',
            ),
            'post_date' => array(
              'weight' => '-19',
              'format' => '',
              'region' => 'header',
              'label_value' => 'Post date',
              'labelformat' => 'hidden',
            ),
            'body' => array(
              'weight' => '-20',
              'format' => 'nd_bodyfield',
              'region' => 'middle',
              'label_value' => 'Core body',
              'labelformat' => 'hidden',
            ),
            'terms' => array(
              'weight' => '-19',
              'format' => '',
              'region' => 'disabled',
              'label_value' => 'Taxonomy',
              'labelformat' => 'hidden',
            ),
            'test_key' => array(
              'weight' => '-20',
              'format' => '',
              'region' => 'footer',
              'label_value' => 'Test',
              'labelformat' => 'hidden',
            ),
          ),
          'status' => 1,
        ),
      ),
    ),
  );
  return $data;
}

function _ndtest_ds_fields() {
  $data = array(
    'nd' => array(
      'test_key' => array(
        'title' => 'Test',
        'code' => '<?php
echo "Test key: ". $object->nid;
?>',
        'exclude' => array(
          'story' => 'story',
          'page' => 0,
        ),
        'type' => 6,
      ),
    ),
  );
  return $data;
}
