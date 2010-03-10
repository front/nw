<?php
//check column
$preffix = '';
if ($left) {
	$preffix .= '-left';
}
if ($right) {
	$preffix .= '-right';
}
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language->language ?>" lang="<?php print $language->language ?>" dir="<?php print $language->dir ?>">
  <head>
    <?php print $head ?>
    <title><?php print $head_title ?></title>
    <?php print $styles ?>
	 <!--[if lte IE 6]><style type="text/css" media="all">@import "<?php print $base_path . path_to_theme() ?>/css/ie6.css";</style><![endif]-->
    <!--[if IE 7]><style type="text/css" media="all">@import "<?php print $base_path . path_to_theme() ?>/css/ie7.css";</style><![endif]-->
	<link type="text/css" rel="stylesheet" media="all" href="<?php print $base_path . path_to_theme() ?>/superfish.css" />
	<script type="text/javascript" src="<?php print $base_path . path_to_theme() ?>/superfish.js"></script>
    <?php print $scripts ?>
	<script type="text/javascript">
    /* <![CDATA[ */
    $(document).ready(function() {
        $('ul.sf-menu').superfish({
            delay:       800,                            // one second delay on mouseout
            animation:   {opacity:'show',height:'show'},  // fade-in and slide-down animation
            speed:       'fast',                          // faster animation speed
            autoArrows:  false,                           // disable generation of arrow mark-up
            dropShadows: true                            // disable drop shadows
        });
     });
     /* ]]> */
    </script>
  </head>
  <body id="norwood" class="<?php print $body_classes; ?>">
  <div id="wrapper">
  
  <!-- header wrap/ -->
  <div id="header">
    <h1 id="logo"><a href="<?php echo base_path(); ?>"><?php echo $site_name; ?></a></h1>
	<!--<small><?php echo $site_slogan; ?></small> -->
	 <!-- top navigation wrap/ -->
    <div id="navigation">
    <?php if ($header) : ?>
      <?php print $header; ?>
	<?php else: ?>
	  <?php print theme('links', $primary_links, array('class' => 'sf-menu')) ?>
    <?php endif; ?>
	<?php if (isset($secondary_links)) : ?>
       <?php print theme('links', $secondary_links, array('class' => 'secondary-links')) ?>
    <?php endif; ?>
    </div>
    <!-- /top navigation wrap -->
  </div>
  <!-- /header wrap -->
  
   <!-- header image/ -->
  <?php if (isset($banner)) : ?>
  <div id="banner">
	<?php print $banner; ?>
  </div>
  <?php endif; ?>
  <!-- /header image -->
  
  <!-- main content wrap/ -->
  <div id="content-wrapper" class="colwrapper<?php print $preffix; ?>"><div id="content-wrapper-bg">
    <?php if ($left): ?>
	<div id="sidebar">
	<?php print $left ?>
	</div>
	<?php endif; ?>
	<div id="maincolumn" class="coltype<?php print $preffix; ?>">
    <?php if ($title): print '<h2'. ($tabs ? ' class="with-tabs heading"' : ' class="heading"') .'>'. $title .'</h2>'; endif; ?>
    <?php if ($show_messages && $messages): print $messages; endif; ?>   
	<?php print $help; ?>
    <?php if ($tabs): print '<div class="tabs">'. $tabs .'</div>'; endif; ?>
	<?php print $content ?>
	</div>
	<?php if ($right): ?>
	<div id="secondbar">
	<?php print $right ?>
	</div>
	<?php endif; ?>
  </div></div>
  <!-- /main content wrap -->
  
  <!-- footer wrap/ -->
  <hr class="hide" />
  <div id="footer-wrapper">
	<?php if ($search_box): ?>
	<div id="search-box">
	<?php print $search_box ?>
	</div>
	<?php endif; ?>
	<?php if ($footer): ?>
	<?php print $footer ?>
	<?php else: ?>
    <div id="nyheter-mer"><a href="/nyheter">Les flere nyheter</a></div>
    <div id="nyheter-eldre"><a href="/nyheter">Les flere nyheter</a></div>
		<!-- <div id="link-sitemap"><a href="#">Sitemap</a></div> -->
		<div id="link-sponsor"><span id="sponsor-text">VÅRE SAMARBEIDSPARTNERE:</span> <span id="sponsor-louis">Louis Vidor</span> <span id="sponsor-dagbladet">Dagbladet</span> <span id="sponsor-sony">Sony Ericsson</span> <span id="sponsor-peppes">Peppes Pizza</span></div>
	<?php endif; ?>
  </div>
  <!-- /footer wrap -->
  
  </div>
  <div id="leaf"></div>
  <?php print $closure ?>
  </body>
</html>
