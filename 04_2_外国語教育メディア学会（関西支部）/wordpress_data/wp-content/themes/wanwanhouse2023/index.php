<?php
/*
デフォルトのテンプレート
*/
get_header();
?>
	<main>
		<section class="">
			<div class="section-padding content-container-md">

				<?php if(have_posts()) {
					while(have_posts()) {
						the_post();
						?>
						<article class="post-container">
							<h2 class="text-center"><?php the_title() ?></h2>
							<div class="post-content">
								<?php the_content() ?>
							</div>
						</article>
						<?php
					}
					?>
				<?php } else { ?>
					<p>記事がありません。</p>
				<?php } ?>
			</div>
		</section>
	</main>

<?php
get_footer();