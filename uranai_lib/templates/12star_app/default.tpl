{extends file='app_main.tpl'}
{block name=body}
<section class="app-nav">
	<ul class="list-inline menu">
		<li class="{if $data_type == ""}nav-selected{/if}"><a href="?date={$date_num}{if $debug_args}{$debug_args}{/if}" class="defolt">全体運</a></li>
		<li class="{if $data_type == "love"}nav-selected{/if}"><a href="?date={$date_num}&data_type=love{if $debug_args}{$debug_args}{/if}" class="love">恋愛運</a></li>
		<li><a href="?mode=kiyaku{if $debug_args}{$debug_args}{/if}" class="contract">規約</a></li>
		<li><a href="#" class="configuration">テーマ</a></li>
	</ul>
	<!--modal-->
	<div class="config-modal">
		<span>ページデザイン変更</span>
		<ul>
		  <li><a href="" class="theme-main">メインテーマ</a></li>
		  <li><a href="" class="theme-chic">シック</a></li>
		</ul>
	</div>
	<!--/modal-->
</section>
<div class="container {$star_name_eng}">
	<section>
		<div class="bk-img">
			<div class="character">
				<h1>{$star_name}　今日の星座占い{if $data_type}<span class="word">（{$topic_name}）</span>{/if}</h1>
				<small>良い結果だけを見たい<span class="word">あなたに・・・</span><br>毎日ハッピーに過ごしましょう！</small>
			</div>
		</div>
	</section>
	<div class="content">
		<section>
			<div id="today-best" class="clearfix">
			<div class="section-title">今日の{if $data_type != ""}{$topic_name}が好調なのは{else}ハッピーなサイトは{/if}<span class="word">以下のサイトです<span></div>	
				<div class="good-rank clearfix">
					{if $is_log}{* logレコードが1件以上 *}
						{if $best_rank_sites}
							{foreach $best_rank_sites as $site}
								<div class="good-list"><a href="{$site.site_url}" target='_blank'>{$site.site}</a></div>
							{/foreach}
						{else}
							<div class='in-progress'><p>現在閲覧可能なサイトがありません。</p></div>
						{/if}
					{else}{* logレコードが0件 *}
						{if $plugin_run}{* 一番早いプラグインが実行されているのにデータがない(エラー) *}
							<div class="in-progress"><p>集計データがありません。</p></div>
						{else}{* プラグイン実行前 *}
							<div class='in-progress'><p>現在データを集計中です。申し訳ありませんが時間をおいてからアクセスしてみてください。</p></div>
						{/if}
					{/if}
				</div>
			</div>
		</section>
		<section>
			<div id="karakuchi-container" class="text-center" data-date="{$date_num}" data-data_type="{$data_type}" data-star="{$star_num}">
				<div id="karakuchi" class="section-title content-closed">すべての結果を見たい場合はこちら</div>
				<div id="karakuchi-content"><!--ajax response here--></div>
			</div>
		</section>
	</div>
</div>
<!--modal-->
<div class="modal " id="confirm" tabindex="-1">
	<div class="modal-dialog">
		<div class="modal-content">
			<div class="modal-header">
				<button type="button" class="close" data-dismiss="modal"><span>×</span></button>
				<h4 class="modal-title">本当に表示させますか？</h4>
			</div>
			<div class="modal-footer">
				<button type="button" id="confirm-yes" class="btn btn-danger">はい！</button>
				<button type="button" class="btn btn-default" data-dismiss="modal">やっぱりやめる</button>
			</div>
		</div>
	</div>
</div>
<!--/modal-->
{/block}
