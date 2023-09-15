<?php
/*
投稿一覧用のテンプレート
*/
get_header();
?>
	<main>
		<section>
			<div class="top-header">
				<div class="header-content">
					<div class="top-left rounded-right top-header-blog-bg">
						<div class=" top-header-container content-container-md">
							<h2 class="top-header-title"><strong>お知らせ</strong></h2>
						</div>
					</div>
					<?php get_template_part("shared/parts/header", "time-information") ?>
				</div>
			</div>
		</section>

		<section class="posts section-padding">
			<div class="content-container-md blog-cols">
				<?= posts_categories_links() ?>

				<?php if(have_posts()) { ?>
					<ul class="posts-list">
						<?php while(have_posts()) {
							the_post();
							$category = get_the_category()[0];
							$slug = $category->slug;
							$cate = $category->name;
							?>
							<li class="post-entry">
								<div class="post-header">
									<span class="post-date"><?= get_the_date(POST_DATE_FORMAT) ?></span>
									<?= post_category_label($slug, $cate) ?>
								</div>
								<div class="post-title">
									<a href="<?= get_the_permalink() ?>"><?= get_the_title() ?></a>
								</div>
								<div class="post-abstract">
									<?= the_excerpt() ?>
								</div>
								<div class="post-readmore">
									<a href="<?= get_the_permalink() ?>">続きを読む &gt;</a>
								</div>
							</li>
							<?php
							}
						?>
					</ul>
				<?php } else { ?>
					<p>記事がありません。</p>
				<?php } ?>
			</div>
			
			<div class="posts-pagination">
				<?php
				echo paginate_links(array(
					'prev_text' => '«',
					'next_text' => '»',
					'end_size' => 2,
					'mid_size' => 2,
					'type'    => 'list'
				));
				?>
			</div>
		</section>
	</main>

<?php
get_footer();