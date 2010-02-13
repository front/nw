<div id="node-<?php print $node->nid; ?>" class="node<?php if ($sticky) { print ' sticky'; } ?><?php if (!$status) { print ' node-unpublished'; } ?>">
  <?php if ($page == 0): ?>
  <h2 class="heading"><a href="<?php print $node_url ?>" title="<?php print $title ?>"><?php print $title ?></a></h2>
  <?php endif; ?>
  <div class="node-content">
  <?php print $content ?>
  </div>
  <?php if ($links): ?>
  <div class="links"><?php print $links; ?></div>
  <?php endif; ?>
</div>
