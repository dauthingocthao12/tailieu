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
				<div class="top-left rounded-right top-header-qa-bg">
					<div class=" top-header-container content-container-md">
						<h2 class="top-header-title"><strong>よくあるご質問</strong> Q&amp;A</h2>
					</div>
				</div>
				<?php get_template_part("shared/parts/header", "time-information") ?>
			</div>
		</div>
	</section>

	<section class="content-container-md section-padding">
		<p class="qa-intro">
			当店を安心してお使いいただくため、 よくある質問にお答えします。 こちらに記載の無いご質問はメールフォーム、またはお電話でお問い合わせください。
		</p>

		<ul class="qa-menu">
			<li><a href="#trimming" class="qa-cate1">トリミング</a></li>
			<li><a href="#shampoo-car" class="qa-cate2">犬の移動美容室</a></li>
			<li><a href="#dog-hotel" class="qa-cate3">ドッグホテル</a></li>
			<li><a href="#pet-taxi" class="qa-cate4">ペットタクシー</a></li>
		</ul>

		<div class="sep-50"></div>

		<section>
			<a name="trimming"></a>
			<h3 class="qa-title text-center">トリミング</h3>

			<dl class="qa-list">
				<?php
				$qa_entries = qa_list("trimming");
				if(count($qa_entries)===0) {
					echo "<p class='text-center'>情報がありません</p>";
				}
				foreach($qa_entries as $entry) {
					/** @var WP_Post $entry */
					echo '<div class="qa-group">';
						echo "<dt>{$entry->post_title}</dt>";
						echo "<dd>{$entry->post_content}";
						echo '<div class="post-thumbnail">';
							echo get_the_post_thumbnail($entry);
						echo '</div>';
					echo "</dd>";
					echo '</div>';
				}
				?>
			</dl>
		</section>

		<div class="sep-50"></div>

		<section>
			<a name="shampoo-car"></a>
			<h3 class="qa-title text-center">犬の移動美容室</h3>

			<dl class="qa-list">
				<?php
				$qa_entries = qa_list("shampoo-car");
				if(count($qa_entries)===0) {
					echo "<p class='text-center'>情報がありません</p>";
				}
				foreach($qa_entries as $entry) {
					/** @var WP_Post $entry */
					echo '<div class="qa-group">';
						echo "<dt>{$entry->post_title}</dt>";
						echo "<dd>{$entry->post_content}";
							echo '<div class="post-thumbnail">';
								echo get_the_post_thumbnail($entry);
							echo '</div>';
						echo "</dd>";
					echo '</div>';
				}
				?>
			</dl>
		</section>

		<div class="sep-50"></div>

		<section>
			<a name="dog-hotel"></a>
			<h3 class="qa-title text-center">ドッグホテル</h3>

			<dl class="qa-list">
				<?php
				$qa_entries = qa_list("dog-hotel");
				if(count($qa_entries)===0) {
					echo "<p class='text-center'>情報がありません</p>";
				}
				foreach($qa_entries as $entry) {
					/** @var WP_Post $entry */
					echo '<div class="qa-group">';
						echo "<dt>{$entry->post_title}</dt>";
						echo "<dd>{$entry->post_content}";
							echo '<div class="post-thumbnail">';
								echo get_the_post_thumbnail($entry);
							echo '</div>';
						echo "</dd>";
					echo '</div>';
				}
				?>
			</dl>
		</section>

		<div class="sep-50"></div>

		<section>
			<a name="pet-taxi"></a>
			<h3 class="qa-title text-center">ペットタクシー</h3>

			<dl class="qa-list">
				<?php
				$qa_entries = qa_list("pet-taxi");
				if(count($qa_entries)===0) {
					echo "<p class='text-center'>情報がありません</p>";
				}
				foreach($qa_entries as $entry) {
					/** @var WP_Post $entry */
					echo '<div class="qa-group">';
						echo "<dt>{$entry->post_title}</dt>";
						echo "<dd>{$entry->post_content}";
							echo '<div class="post-thumbnail">';
								echo get_the_post_thumbnail($entry);
							echo '</div>';
						echo "</dd>";
					echo '</div>';
				}
				?>
			</dl>
		</section>

	</section>

	<?= get_template_part("shared/parts/common", "banners") ?>
</main>

<?php
get_footer();
