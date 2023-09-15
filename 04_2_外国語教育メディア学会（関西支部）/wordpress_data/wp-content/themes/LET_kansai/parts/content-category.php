
<article id="post-<?php the_ID(); ?>" <?php post_class(); ?>>

	<div class="entry-content">
		<h3><?php the_title() ?><small><?php the_date() ?></small></h3>
		<?php the_content() ?>
	</div><!-- .entry-content -->

</article><!-- #post-<?php the_ID(); ?> -->
