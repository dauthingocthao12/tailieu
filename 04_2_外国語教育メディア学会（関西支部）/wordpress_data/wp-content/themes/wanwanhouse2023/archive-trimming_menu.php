<?php
/*
トリミングメニューの一覧用のテンプレート
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

		<section class="posts section-padding">
			<div class="content-container-md">
				<?php if(have_posts()) { ?>
					<ul class="trimming-menu posts-list">
						<?php while(have_posts()) {
							the_post();
							?>
							<li class="post-entry">
								<a href="<?= get_the_permalink() ?>">
								<figure>
									<?= get_the_post_thumbnail() ?>
									<figcaption><?= get_the_title() ?></figcaption>
								</figure>
								</a>
							</li>
							<?php
							}
						?>
					</ul>
				<?php } else { ?>
					<p>記事がありません。</p>
				<?php } ?>
			</div>
		</section>
	</main>

<?php
get_footer();