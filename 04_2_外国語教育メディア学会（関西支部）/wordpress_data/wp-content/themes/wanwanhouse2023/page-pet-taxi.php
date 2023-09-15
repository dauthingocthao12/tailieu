<?php
/*
    ペットタクシー ページ用のテンプレート
*/

get_header();
?>
<main class="taxi">
	<section>
		<div class="top-header">
			<div class="header-content">
				<div class="top-left rounded-right top-header-taxi-bg">
					<div class=" top-header-container content-container-md">
						<h2 class="top-header-title"><strong>ペットタクシー</strong></h2>
					</div>
				</div>
				<?php get_template_part("shared/parts/header", "time-information") ?>
			</div>
		</div>
	</section>

	<section class="section-lb-pink">
		<div class="content-container-md section-padding">
			<h2>ペットタクシー</h2>
			<div class="sep-25"></div>
			<div class="taxi-intro">
				<div class="intro-txt">
					<p>
						「 狂犬病の予防接種や専門的治療のできる病院へ連れて行きたい 」<br>
						「 遠く離れた場所に住んでいる子どもが飼っている愛犬の送迎をしてほしい 」
					</p>
					<p>
						当店では上記のような実際にお寄せいただいたお客様の声にお応えするため 、 ペットタク
						シー事業を開始いたしました 。<br>
						ペットタクシーとは 、 ペットの快適で安全な移動のための送迎サービスです 。 飼い主様も
						2 名まで同乗する事が可能です 。 ( 飼い主様はペットの付添い人として同乗し 、 料金は発生
						しません ) 当店は 、 国土交通省陸運局届済貨物軽自動車運送事業の認可を取得してお
						ります 。<br>
						専門知識をもつスタッフが対応しますので 、 安心してご利用いただけます 。
					</p>
				</div>
				<div class="intro-pic">
					<img src="<?= get_template_directory_uri() ?>/shared/images/taxi/intro-img.jpg" alt="タクシーの写真" class="image-responsive">
				</div>
			</div>
		</div>
	</section>
	<section class="section-leaf-pink">
		<div class="content-container-md section-padding">
			<h2>利用例</h2>
			<div class="sep-25"></div>
			<ul class="points-list">
				<li>動物病院に連れて行きたいけど頼める人がいない</li>
				<li>専門的治療のできる動物病院へ通院したい</li>
				<li><a href="<?= home_url("hotel") ?>">ペットホテル</a>や<a href="<?= home_url("trimming") ?>">トリミング</a>、ペットスクール利用時の送迎をして欲しい</li>
				<li>ペットと一緒に買い物やお出かけ、旅行に行きたい</li>
				<li>引っ越し先まで連れて行ってほしい</li>
				<li>空港、駅まで送ってほしい</li>
			</ul>
			※ご利用は片道、往復のどちらでも対応可能です。また、ペットのみの送迎も承ります。
			<div class="sep-25"></div>
			<p>
				ペットの移動でお困りの飼い主様やご家族の方または知人でお困りの方はご相談の電話をお待ちしております。<br>
				当店もペットタクシー事業は新たな挑戦となり手探り状態ではありますが　少しでも困っているお客様のお役に立てる様に一生懸命に対応させていただきます。
			</p>

			<div class="sep-25"></div>
			<?= get_template_part("shared/parts/common", "hours-bloc"); ?>

		</div>
	</section>

	<section class="section-lb-rt-pink"">
		<div class="section-padding content-container-md">
			<h2>料金について</h2>
			<div class="sep-25"></div>
			<section>
				<h3>実走行距離プラン</h3>
				<p>基本料金 ＋ お迎え料金 ＋ 距離増料金 (走行距離11km以上の場合) ＋ 待機料金</p>
				<dl class="taxi-prices">
					<dt>基本料金</dt>
					<dd>10kmまで3,800円</dd>

					<dt>お迎え料金</dt>
					<dd>
						<p>
							5km以内は1,000円、超過分は155円/km加算となります。<br>
							（出発地点：群馬県前橋市岩神町3-12-19 ワンワンハウス）
						</p>
					</dd>

					<dt>距離増料金</dt>
					<dd>
						<table>
							<thead><tr>
								<th>走行距離</th>
								<th>増料金</th>
							</tr></thead>
							<tbody>
							<tr>
								<td>11km～100km</td>
								<td class="text-right">200円／km</td>
							</tr>
							<tr>
								<td>101km～200km</td>
								<td class="text-right">182円／km</td>
							</tr>
							<tr>
								<td>201km～</td>
								<td class="text-right">164円／km</td>
							</tr>
							</tbody>
						</table>
					</dd>

					<dt>待機料金</dt>
					<dd>30分ごとに1,500円</dd>
				</dl>
			</section>

			<div class="sep-25"></div>
			<section>
				<h3>貸し切りプラン</h3>
				<p>コース料金 ＋ 時間超過料金 ＋ 距離超過料金</p>

				<dl class="taxi-prices">
					<dt>15,000円コース</dt>
					<dd>ご利用時間5時間以内 総走行距離数70kmまで 15000円</dd>

					<dt>30,000円コース</dt>
					<dd>ご利用時間10時間以内 総走行距離160kmまで 30000円</dd>

					<dt>時間超過料金</dt>
					<dd>30分毎に1,500円加算となります。</dd>

					<dt>距離超過料金</dt>
					<dd>１km毎に155円加算となります。</dd>
				</dl>
			</section>

			<div class="sep-25"></div>

			<div class="content-box line-colored main-text">
				<ul class="taxi-caution">
					<li>価格はすべて税別価格となります。</li>
					<li>有料道路利用料、駐車料金は別途加算とします。</li>
					<li>午後10時から午前5時までにご利用いただいた際には早朝、深夜割増として料金の3割を追加加算させていただきます。</li>
				</ul>
			</div>

			<div class="sep-50"></div>

			<div class="read-more">
				<a href="#qa">ペットタクシーに関するよくある質問<span class="dli-arrow-right"></span></a>
			</div>
		</div>
	</section>

	<section style="background-color:var(--bg-light)">
		<div class="section-padding content-container-md">
			<h2>ご予約の4ステップ</h2>
			<div class="sep-25"></div>

			<dl class="content-flow">
				<div class="content-box noline-nocolored text-center">
					<dt>STEP1</dt>
					<dd>
						ご利用は予約制となります。まずは電話にてご相談下さい。<br>
						Tel 027-234-5400 （毎週木曜日定休 営業時間9：00～19：00）
					</dd>
				</div>

				<div class="triangle-big text-center"></div>

				<div class="content-box noline-nocolored text-center">
					<dt>STEP2</dt>
					<dd>
						サービスに必要な下記情報をお教えください。<br>
						<ul class="points-list">
							<li>お客様のお名前、ご連絡先</li>
							<li>ご希望日時</li>
							<li>お迎え場所、目的地</li>
							<li>乗車されるペットのお名前や犬種、体重等</li>
							<li>付添人同乗の有無</li>
							<li>キャリーやブランケット等のご用意</li>
							<li>狂犬病ワクチン接種の確認（当日ワクチン接種が証明できるものをご用意いただきます）</li>
						</ul>
					</dd>
				</div>

				<div class="triangle-big text-center"></div>

				<div class="content-box noline-nocolored text-center">
					<dt>STEP3</dt>
					<dd>
						頂いた情報を元にお見積りさせていただきます。
					</dd>
				</div>

				<div class="triangle-big text-center"></div>

				<div class="content-box noline-nocolored text-center">
					<dt>STEP4</dt>
					<dd class="text-left">
						サービス内容にご納得いただけましたら予約確定となります。<br>
						尚、ご利用当日に同意書のご署名もお願いしております。<br>
						☆午後10時から午前5時までにご利用いただいた際には早朝、深夜割増として料金の3割を追加加算させていただきます。
					</dd>
				</div>
			</dl>
		</div>
	</section>

	<section class="section-rt-pink">
		<div class="section-padding content-container-md">
			<h2>ご利用時の注意事項</h2>
			<div class="sep-25"></div>

			<ul class="points-star">
				<li>当店は国土交通省認可を受けた正規のペットタクシーとなります。</li>
				<li>車両数の制限がありますので、余裕をもってご予約下さい。</li>
				<li>ご利用料金はペット降車時に現金にてご清算ください。ペットだけのご利用の際は、乗車地にてお見積り額をお支払いいただきます。</li>
				<li>病院で定期的に予防接種を受けているペットのみご利用いただけます。</li>
				<li>ご利用料金は、お見積りさせていただいた金額を請求させていただきます。但し、お客様のご都合による経路の変更、待機時間の延長、深夜割増、別途実費負担費用等が発生した場合には、追加分を含めた金額を請求させていただきます。</li>
				<li>午後10時から午前5時までにご利用いただいた際には早朝、深夜割増として料金の3割を追加加算させていただきます。</li>
				<li>予約前日までのキャンセル手数料は発生しません。但し、サービス当日にキャンセルされた場合には、キャンセル料3,000円を頂戴いたします。</li>
				<li>サービスにおいて危険を伴う恐れのあるペット、ご希望に対してはご利用をお断りさせていただく場合がございます。</li>
				<li>指定の時刻通り運行できるよう最善を尽くします。但し、当日の道路状況、車両の故障・事故などに起因しご希望に添えなかった場合の運賃の割引、補償、慰謝料等の請求には応じかねますのでご了承ください。</li>
				<li>ペット輸送の際に、万一、荷受人様がペットの受取を拒否された場合には、ペットを荷送人様の元へご返送させていただきます。また、その場合の返送に伴う料金は荷送人様のご負担となります。</li>
				<li>万一、不慮の事故でペットがケガや病気、死亡した場合、損害賠償及び慰謝料等の請求には応じかねますのでご了承ください。なお、同乗者につきましては「事業活動包括保険」「運送貨物賠償責任特約」の範囲で対応させていただきます。詳細は下記の〈ペットタクシーの保険と補償〉をご確認ください。</li>
				<li>感染症等の疑いのあるペットが乗車する場合には、ご予約時にお伝えください。</li>
				<li>ペット輸送は貨物自動車運送事業施行規則に基づく標準貨物軽自動車運送約款により、ペットは貨物扱いとなりますので、飼い主様のみの移動はできません。</li>
			</ul>

			<div class="sep-25"></div>

			<h3>〈ペットタクシーの保険と補償〉</h3>
			<dl class="taxi-prices">
				<dt>ペットに対して</dt>
    		<dd>
					サービス利用中や車両乗車中のペットが｢火災･爆発または輸送用具の衝突｣により､万一死亡された場合｢事業活動包括保険｣｢運送貨物賠償責任特約｣の補償を受けることができます。 なお、ケガに対する補償はできませんのでご了承ください。 ペットの対応には十分留意しますが､万一､不可抗力等により下記のような事故が発生した場合の補償・慰謝料等の請求には応じかねますのでご了承ください｡<br>
					<ul class="points-list">
						<li>乗車中のペットの事故（ケガ、逃亡など）※死亡の場合は保険の補償金の範囲内で適用されます。</li>
						<li>乗車中のペットの発病</li>
						<li>降車後のペットの発病、死亡</li>
					</ul>
				</dd>

				<dt>付添人に対して</dt>
    		<dd>
					付添人として同乗された方が、万一死傷された場合、「対人賠償責任保険」及び「搭乗者傷害保険」の補償を補償金の範囲内で受けることができます。
				</dd>
			</dl>
		</div>
	</section>

	<a name="qa"></a>
	<section class="section-padding" style="background-color: var(--bg-light);">
		<div class="content-container-md">
			<h2>よくある質問</h2>
			<div class="sep-50"></div>

			<dl class="qa-list">
				<?php
				$qa_entries = qa_list("pet-taxi");
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
