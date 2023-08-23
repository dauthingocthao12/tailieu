{extends file="main.tpl"}
{block name=seo}
<title>順位一覧 | 12星座占いランキング</title>
<meta name="keywords" content="12星座占い,せいざうらない,12星座,せいざ,順位一覧,uranairanking.jp">
<meta name="description" content="ユーザー登録者様用順位一覧ページです">

<!--OGP START-->
{include file="ogp.tpl" title="|マイページ" des="ユーザー登録者様用順位一覧ページです"}
<!--OGP END-->
{/block}



{block name=body}
<div class="container registered-person">
{if $login_ok}
	<div class="title row text-center">
		<h2 class="font-color"><span class="open_date">{$open_day}</span><span class="word-break">現在の順位一覧</span></h2>
		{include "mainline.parts.tpl"}
	</div>

	<div class="spadding-top">
		<div class="tecen base-bg contents-space">
			<table class="registered-person-ranking">
				<tr class="unsei-list">
				<th colspan="2" class="table-sep"><span class="unsei">運勢</span><span class="seizamei">星座名</span></th><th class="defolt tb-width">総合運</th><th class="love tb-width">恋愛運</th><th class="work tb-width">仕事運</th><th class="money tb-width">金運</th>
				</th>
				<tr class="unsei-koumoku">
				</tr>
				{for $sn = 1 to 12}

					<tr class="{if {$sn%2} == 0}star-name-list{/if} {if {$sn} == $user_star}user-color{/if}">
						<th class="star-name " rowspan = "2">{if {$sn} == $user_star}<span class="user-mark">★</span>{/if}{$jpn_num_star.$sn}</th>
						<td>順位</td>
						<td>{$updown_rank.$sn.defolt.ranking}</td>
						<td>{$updown_rank.$sn.love.ranking}</td>
						<td>{$updown_rank.$sn.work.ranking}</td>
						<td>{$updown_rank.$sn.money.ranking}</td>
					</tr>
					<tr class="{if {$sn%2} == 0}star-name-list{/if} {if {$sn} == $user_star}user-color{/if}">
						<td>前日比</td>
						<td>{$updown_rank.$sn.defolt.mark}</td>
						<td>{$updown_rank.$sn.love.mark}</td>
						<td>{$updown_rank.$sn.work.mark}</td>
						<td>{$updown_rank.$sn.money.mark}</td>
					</tr>

				{/for}
			</table>
		</div>
	</div>
{else}
	<div class="tecen base-bg contents-space login-necessary">
	<h3>順位一覧ページを見るには、ログインが必要です。</h3>
	<p>ログイン後『マイページ → 順位一覧ページを見る』で閲覧して下さい</p>
	<div>
	<a href="{sitelink mode="account/login"}" class="btn btn-primary">ログインする</a>
	</div>
	</div>
{/if}
</div>
{/block}