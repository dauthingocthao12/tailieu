<?php
/*
	お問い合わせページ用のテンプレート
*/

get_header();
?>
<main>
	<section>
		<div class="top-header">
			<div class="header-content">
				<div class="top-left rounded-right contact-top">
					<div class=" top-header-container content-container-md">
						<h2 class="top-header-title"><strong>お問合せ</strong></h2>
					</div>
				</div>
				<?php get_template_part("shared/parts/header", "time-information") ?>
			</div>
		</div>
	</section>

	<section class="">
		<div class="content-container-md section-padding">
			<h2><?php the_title() ?></h2>
			<div class="sep-50"></div>

			<div class="contact  main-text ">
				<div class="line-colored text-center text-left">
				以下の方法でお問合せください。
					<div class="sep-25"></div>
					<div class="flex-box">&#x25BA;&#xFE0E; 電話でのお問合せ</div>
					<div class="sep-10"></div>
					<div>
						受付時間：9：00〜19：00 定休日：毎週木曜日
					</div>
					<div>
					<a href="tel:0272345400">TEL 027-234-5400</a>
					</div>
					<div class="sep-25"></div>
					<div class="flex-box">&#x25BA;&#xFE0E; メールでのお問合せ</div>
					<div>
					<a href="mailto:info@wan-wan-house.com?subject=お問合せ">info@wan-wan-house.com</a>
					</div>
					<div>
					info@wan-wan-house.comの受信を可能な設定にして、お問合せ下さい。
					</div>
					<div class="sep-25"></div>
				</div>
				<div class="sep-25"></div>
			</div>
		</div>
	</section>

	<?= get_template_part("shared/parts/common", "banners") ?>
</main>

<?php
get_footer();
