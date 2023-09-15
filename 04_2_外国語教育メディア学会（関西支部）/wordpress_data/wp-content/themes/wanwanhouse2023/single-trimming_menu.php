<?php
/*
トリミングメニュー詳細用のテンプレート
*/
get_header();
?>
<main>
	<section>
		<div class="top-header">
			<div class="header-content">
				<div class="top-left rounded-right top-header-blog-bg">
					<div class=" top-header-container content-container-md">
						<h2 class="top-header-title"><strong>トリミングメニュー</strong></h2>
					</div>
				</div>
				<?php get_template_part("shared/parts/header", "time-information") ?>
			</div>
		</div>
	</section>

	<section class="post">
		<div class="section-padding content-container-md">

			<?php if(have_posts()) {
				while(have_posts()) {
					the_post();
					?>
					<article class="post-container">
						<h2 class="post-title"><?php the_title() ?></h2>

						<div class="post-trimming-menu">
							<div class="post-thumbnail">
								<?= get_the_post_thumbnail() ?>
							</div>
							<div class="post-content">
								<?php the_content() ?>
							</div>
						</div>
					</article>
					<?php
				}
				?>
				<div class="sep-50"></div>
				<div class="text-center">
					<a href="javascript:history.back();">前のページに戻る</a>
				</div>
			<?php } else { ?>
				<p>記事がありません。</p>
			<?php } ?>
		</div>
	</section>
</main>

<?php
get_footer();