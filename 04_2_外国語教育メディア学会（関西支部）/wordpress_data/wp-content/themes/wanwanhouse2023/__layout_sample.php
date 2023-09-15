<?php
/*
    グルーミング ページ用のテンプレート
*/

get_header();
?>
<main>
	<section>
		<div class="top-header">
			<div class="header-content">
				<div class="top-left rounded-right" style="background-color:blue">
					<div class=" top-header-container content-container-md">
						<h2 class="top-header-title"><strong>トリミング</strong> Trimming</h2>
					</div>
				</div>
				<?php get_template_part("shared/parts/header", "time-information") ?>
			</div>
		</div>
	</section>

	<section class="section-lb-pink" style="height:550px;">
		<div class="content-container-md section-padding">
			<h2><?php the_title() ?></h2>
			<!-- コンテンツです。（テンプレートに固定されています） -->
		</div>
	</section>
	<section class="section-leaf-pink" style="height:550px;">
		<div class="content-container-md section-padding">
			<h2><?php the_title() ?></h2>
			<!-- コンテンツです。（テンプレートに固定されています） -->
		</div>
	</section>

	<section class="section-lb-rt-pink" style="height:550px;">
		<div class="section-padding content-container-md"></div>
	</section>

	<section style="height:550px;background-color:var(--bg-light)">
		<div class="section-padding content-container-md">
			<h2><?php the_title() ?></h2>
			<!-- コンテンツです。（テンプレートに固定されています） -->
		</div>
	</section>

	<section style="height:550px;background-color:var(--bg-light)">
		<div class="section-padding content-container-md">
			<h2><?php the_title() ?></h2>
			<!-- コンテンツです -->
		</div>
	</section>

	<section class="section-rt-pink" style="height:550px;">
		<div class="section-padding content-container-md">
			<h2><?php the_title() ?></h2>
			<!-- コンテンツです -->
		</div>
	</section>

	<?= get_template_part("shared/parts/common", "banners") ?>
</main>

<?php
get_footer();
