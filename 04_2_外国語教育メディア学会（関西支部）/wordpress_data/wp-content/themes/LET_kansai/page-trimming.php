<?php
/*
    グルーミング ページ用のテンプレート
*/

get_header();
?>

<!-- <section>
	<div class="top-header">
		<div class="header-content">
			<div class="top-left rounded-right trimming-top">
				<div class=" top-header-container content-container-md">
					<h2 class="top-header-title"><strong>トリミング</strong></h2>
				</div>
			</div>
			<?php get_template_part("shared/parts/header", "time-information") ?>
		</div>
	</div>
</section> -->

<main class="trimming">
	<?php the_content(); ?>
</main>

<?php
get_footer();
