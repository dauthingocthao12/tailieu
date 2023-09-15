<?php
/*
    採用情報 ページ用のテンプレート
*/

get_header();
?>
<main class="recruit">
    <section>
		<div class="top-header">
			<div class="header-content">
				<div class="top-left rounded-right top-header-recruit-bg">
					<div class=" top-header-container content-container-md">
						<h2 class="top-header-title"><strong>採用情報</h2>
					</div>
				</div>
				<?php get_template_part("shared/parts/header", "time-information") ?>
			</div>
		</div>
	</section>

    <div class="content-container-md section-padding">
	    <p class="recruit-message">ただいま、募集しておりません。</p>
    </div>
	<?= get_template_part("shared/parts/common", "banners") ?>
</main>

<?php
get_footer();
