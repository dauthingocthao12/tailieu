<?php
/*
    ドッグホテル ページ用のテンプレート
*/

get_header();
?>
<main class="hotel">
	<section>
		<div class="top-header">
			<div class="header-content">
				<div class="top-left rounded-right top-header-hotel-bg">
					<div class=" top-header-container content-container-md">
						<h2 class="top-header-title"><strong>ドッグホテル</strong></h2>
					</div>
				</div>
				<?php get_template_part("shared/parts/header", "time-information") ?>
			</div>
		</div>
	</section>

	<section class="section-lb-pink">
		<div class="content-container-md section-padding">
			<h2>ドッグホテルのご案内</h2>
			<div class="sep-25"></div>

			<div class="hotel-intro">
				<div class="intro-txt">
					<p>お預かり、お迎え時間9:00〜19:00</p>
					<p>定休日につきましてはご相談ください。</p>
					<p>ホテル利用時にトリミングをご希望のお客様には延長料金無料サービスがございます。</p>
					<p>小型犬限定:ベーシックコース1500円（7kg以上10kg未満の小型犬＋500円）のお得なメニューもございます。</p>
					<p>店内にフリースペースもご用意しております。ワンちゃん同士の相性もありますので他のワンちゃんと直接遊ばせる事はありません。その子に合った対応をさせていただいております。</p>
					<p>お散歩ご希望の方はお申し付けください。</p>
					<p>別途料金はかかりません。（状況によっては対応できない場合がございます。）</p>
					<p>ホテルのご予約のキャンセル、泊数を残して早めのお迎えでも返金させていただきますのでご安心ください。</p>
					<p>近くの施設</p>
					<p>前橋インター約25分 / ドーム 約5分 / 正田醤油スタジアム約5分 / 群馬県総合スポーツセンター約8分 / 群馬大学医学部附属病院約3分 / 群馬県庁、前橋市役所 約8分 / ベイシア文化ホール約8分 / 伊香保温泉 約34分 / グーグルにて検索</p>
				</div>

				<div class="intro-pic">
					<img src="<?= get_template_directory_uri() ?>/shared/images/hotel/intro-img1.jpg" alt="ドッグホテルの写真" class="image-responsive">
				</div>
			</div>

			<div class="sep-25"></div>
			<?= get_template_part("shared/parts/common", "hours-bloc"); ?>
		</div>
	</section>

	<section class="section-leaf-pink">
		<div class="content-container-md section-padding">
			<h2>ドッグホテルお部屋一覧</h2>
			<div class="sep-25"></div>
			<p>ホテルは見学可能です！ご希望の方はぜひお問合せ下さい</p>

			<div class="hotel-pics">
				<figure>
					<img src="<?= get_template_directory_uri() ?>/shared/images/hotel/ref1.jpg" class="image-responsive" alt="ホテルお部屋一覧">
					<!-- <figcaption>1階全体の様子</figcaption> -->
				</figure>
				<figure>
					<img src="<?= get_template_directory_uri() ?>/shared/images/hotel/ref2.jpg" class="image-responsive" alt="ホテルお部屋一覧">
					<!-- <figcaption>窓際のお部屋</figcaption> -->
				</figure>
				<figure>
					<img src="<?= get_template_directory_uri() ?>/shared/images/hotel/ref3.jpg" class="image-responsive" alt="ホテルお部屋一覧">
					<!-- <figcaption>静かなお部屋もあります</figcaption> -->
				</figure>
				<figure>
					<img src="<?= get_template_directory_uri() ?>/shared/images/hotel/ref4.jpg" class="image-responsive" alt="ホテルお部屋一覧">
					<!-- <figcaption>ご要望により複数同室も可</figcaption> -->
				</figure>
				<figure>
					<img src="<?= get_template_directory_uri() ?>/shared/images/hotel/ref5.jpg" class="image-responsive" alt="ホテルお部屋一覧">
					<!-- <figcaption>小型犬用ゲージ</figcaption> -->
				</figure>
				<figure>
					<img src="<?= get_template_directory_uri() ?>/shared/images/hotel/ref6.jpg" class="image-responsive" alt="ホテルの写真">
					<!-- <figcaption>小型犬～中型犬用ゲージ</figcaption> -->
				</figure>
				<figure>
					<img src="<?= get_template_directory_uri() ?>/shared/images/hotel/ref7.jpg" class="image-responsive" alt="ホテルの写真">
					<!-- <figcaption>体調管理も徹底しています</figcaption> -->
				</figure>
				<figure>
					<img src="<?= get_template_directory_uri() ?>/shared/images/hotel/ref8.jpg" class="image-responsive" alt="ホテルの写真">
					<!-- <figcaption>円形ゲージ</figcaption> -->
				</figure>
				<figure>
					<img src="<?= get_template_directory_uri() ?>/shared/images/hotel/ref9.jpg" class="image-responsive" alt="ホテルの写真">
					<!-- <figcaption>遊べるスペースもあります</figcaption> -->
				</figure>
			</div>
		</div>
	</section>

	<section class="section-lb-rt-pink">
		<div class="section-padding content-container-md">
			<h2>ドッグホテルご利用時に準備していただきたい物</h2>
			<div class="sep-25"></div>

			<div class="hotel-belongings">
				<div class="belongings-pic">
					<img src="<?= get_template_directory_uri() ?>/shared/images/hotel/belongings1.jpg" class="image-responsive" alt="ドッグホテルご利用時に準備していただきたい物">
				</div>
				<div class="belongings-txt">
					<ul class="points-list">
						<li>予防接種が確認できるもの</li>
						<li>首輪・胴輪・リード</li>
						<li>いつもお家で召し上がっている食事(1回ずつ量をわけたもの)</li>
					</ul>
					<p class="belongings-caution"><span class="underline underline-green">高齢犬・持病のあるワンちゃんは必ず以下の物もご用意をお願いいたします。</span></p>
					<ul class="points-list">
						<li>かかりつけの動物病院の診察券、もしくは連絡先</li>
						<li>海外へ行かれる方は、日本国内にお住いのご連絡の取れる方の連絡先</li>
					</ul>
				</div>
			</div>

			<div class="sep-25"></div>

			<div class="read-more">
				<a href="#qa">ペットホテルに関するよくある質問<span class="dli-arrow-right"></span></a>
			</div>
		</div>
	</section>

	<section style="background-color:var(--bg-light);">
		<div class="section-padding content-container-md">
			<h2>ドッグホテル料金表</h2>
			<div class="sep-25"></div>

			<table class="hotel-prices">
				<thead>
					<tr>
						<th>種　類</th>
						<th>1泊(24時間まで)</th>
						<th>1日(日帰り 4時間以上)</th>
						<th>半日(4時間まで)</th>
					</tr>
				</thead>
				<tbody>
					<tr>
						<th>小型犬</th>
						<td class="text-right" title="1泊(24時間まで)">￥3,000</td>
						<td class="text-right" title="1日(日帰り 4時間以上)">￥2,000</td>
						<td class="text-right" title="半日(4時間まで)">￥1,500</td>
					</tr>
					<tr>
						<th>小型犬(7kg以上、10kg未満)<br>10kg以上は中型犬料金になります</th>
						<td class="text-right" title="1泊(24時間まで)">￥3,500</td>
						<td class="text-right" title="1日(日帰り 4時間以上)">￥2,500</td>
						<td class="text-right" title="半日(4時間まで)">￥1,750</td>
					</tr>
					<tr>
						<th>中型犬</th>
						<td class="text-right" title="1泊(24時間まで)">￥4,000</td>
						<td class="text-right" title="1日(日帰り 4時間以上)">￥3,000</td>
						<td class="text-right" title="半日(4時間まで)">￥2,000</td>
					</tr>
				</tbody>
			</table>

			<div class="sep-25"></div>
			<ul class="points-list">
				<li>ホテル中にご希望の方はお散歩サービス付き（ホテル料金に含まれます)</li>
				<li>短時間のお預かりもしておりますので、お気軽にご相談ください</li>
				<li>ホテル利用時に、トリミング(犬のシャンプー・カット)等をご希望のお客様はご予約の際にお申しつけください。※当日ですと、予約がおとりできない場合がございます。</li>
			</ul>

			<div class="sep-25"></div>
			<ul class="triangle-list">
				<li>トリミングのコース・料金説明は<a href="<?= home_url("trimming") ?>">こちら</a></li>
				<li>小型犬の方はベーシックコースがおススメ！</li>
			</ul>

			<div class="sep-50"></div>

			<h3>犬種内訳</h3>
			<div class="sep-25"></div>

			<table class="table-2col-hl">
				<tbody>
					<tr>
						<th>小型犬</th>
						<td>チワワ、Mダックス、マルチーズ、シーズー、Mシュナウザー、ヨークシャーテリア、パピヨン、ポメラニアン、キャバリア、トイプードル、フレンチブルドッグ</td>
					</tr>
					<tr>
						<th>中型犬</th>
						<td>柴犬、ビーグル、コーギー、コッカー、シェルティ</td>
					</tr>
				</tbody>
			</table>
		</div>
	</section>

	<section class="section-lb-rt-pink">
		<div class="section-padding content-container-md">
			<h2>ドッグホテル送迎サービス</h2>
			<div class="sep-25"></div>

			<div class="text-center">
				<p>
					当店ではご希望のお客様にわんちゃんの送迎サービスを行っております。<br>
					ぜひご利用ください！　※詳しくはお電話にてお問い合せください。
				</p>
				<p class="belongings-caution">
					<span class="underline underline-orange">送迎料金　片道 ￥500~</span>
				</p>
				<p>
					ご予約・お問い合せ　TEL. <a href="tel:0272345400">027-234-5400</a>
				</p>
			</div>
		</div>
	</section>

	<section style="background-color:var(--bg-light);">
		<div class="content-container-md section-padding">
			<h2>動物取扱者としての表示</h2>
			<div class="sep-50"></div>

			<table class="table-2col-hl">
				<tbody>
					<tr>
						<th>動物取扱業の種別</th>
						<td>保管</td>
					</tr>
					<tr>
						<th>登録番号</th>
						<td>第010000-16-31号</td>
					</tr>
					<tr>
						<th>登録年月日</th>
						<td>平成18年11月27日</td>
					</tr>
					<tr>
						<th>有効期限の末日</th>
						<td>令和8年11月26日</td>
					</tr>
					<tr>
						<th>動物取扱責任者</th>
						<td>樺澤　みどり</td>
					</tr>
				</tbody>
			</table>
		</div>
	</section>

	<a name="qa"></a>
	<section class="section-padding section-rt-pink">
		<div class="content-container-md">
			<h2>よくある質問</h2>
			<div class="sep-50"></div>

			<dl class="qa-list">
				<?php
				$qa_entries = qa_list("dog-hotel");
				if (count($qa_entries) === 0) {
					echo "<p class='text-center'>情報がありません</p>";
				}
				foreach ($qa_entries as $entry) {
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
		</div>
	</section>

	<?= get_template_part("shared/parts/common", "banners") ?>
</main>

<?php
get_footer();
