<?php
// $Id: ds-display-overview-form.tpl.php,v 1.1.2.6 2010/01/24 17:31:25 swentel Exp $

/**
 * @file
 *   Template file for the display settings overview form
 *
 * @var
 * $build_mode String
 *   Current selected display mode
 * $rows Array of field objects
 *   Fields declared in drupal core and custom fields
 *   Properties (human_name, weight, stickyorder, build_mode, class, label_class)
 */

print $exclude_build_mode;

if ($rows): ?>

<div id="ds-display-content" <?php print $extra_style; ?>>
  <?php if ($plugins): ?>
    <div id="display-tabs">
      <div id="field-tab"><a href="javascript:;" onClick="javascript:toggleFieldPluginsLink('fields-content', 'plugins-content');"><?php print t('Fields'); ?></a></div>
      <div id="plugin-tab"><a href="javascript:;" onClick="javascript:toggleFieldPluginsLink('plugins-content', 'fields-content');"><?php print t('Plugins'); ?></a></div>
    </div>
    <div style="clear: both"></div>
  <?php endif; ?>

  <div id="fields-content">

    <table id="fields" class="sticky-enabled">
      <thead>
        <tr>
          <th><?php print t('Field'); ?></th>
          <th><?php print t('Label'); ?></th>
          <th><?php print t('Format'); ?></th>
          <th><?php print t('Region'); ?></th>
          <th><?php print t('Weight'); ?></th>
        </tr>
      </thead>
      <tbody>

      <!-- Node regions -->
      <?php foreach ($regions as $region => $title): ?>
        <tr class="region region-<?php print $region?>">
          <td colspan="6" class="region"><?php print $title; ?></td>
        </tr>
        <tr class="region-message region-<?php print $region?>-message <?php print empty($rows[$region]) ? 'region-empty' : 'region-populated'; ?>">
          <td colspan="6"><em><?php print t('No fields in this region'); ?></em></td>
        </tr>

        <!-- fields -->
        <?php
        if (!empty($rows[$region])):
          $count = 0;
          foreach ($rows[$region] as $row): ?>
            <tr class="<?php print $count % 2 == 0 ? 'odd' : 'even'; ?> <?php print $row->class ?>">
              <td class="ds-label"><span class="<?php print $row->label_class; ?>"><?php print $row->human_name; ?></span><span class="label-edit"><?php print $row->{$build_mode}->label_edit; ?></span><?php print $row->{$build_mode}->label_value; ?></td>
              <td><?php print $row->{$build_mode}->label; ?></td>
              <td><?php print $row->{$build_mode}->format; ?></td>
              <td><?php print $row->{$build_mode}->region; ?></td>
              <td><?php print $row->ds_weight; ?></td>
            </tr>
            <?php
            $count++;
          endforeach;
        endif;
      endforeach;
      ?>
      </tbody>
    </table>
  </div>
  <?php if ($plugins): ?>
  <div id="plugins-content">
    <?php print $plugins_content; ?>
  </div>
  <?php endif; ?>
</div>
<?php
print $submit;
endif;
