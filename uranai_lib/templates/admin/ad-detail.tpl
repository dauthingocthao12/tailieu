{extends file="main.tpl"}
{* block name=title}{$data.title}{/block *}
{block name=body}
<!--center><a href="../libadmin/edit.php?id={$datum.site_id}">[編集]</a></center-->

{if $data.db}

{if !$data.check}
{* 登録確認以外の場合は *}
<div class="text-right">
	{makelink mode="ad" action="listing" value="一覧へ戻る" class="btn btn-warning"}
	{makelink mode="ad" action="input" id="{$data.db.ad_id}" value="編集" class="btn btn-primary"}
</div>
{/if}

<br>

<table class="table table-bordered">
	<tbody>
		<tr>
			<th width="20%">ID</th>
			<td>{$data.db.ad_id}</td>
		</tr>
		<tr>
			<th>広告名</th>
			<td>{$data.db.ad_name}</td>
		</tr>
		<tr>
			<th>開始日時</th>
			<td>{$data.db.ad_date_begin}</td>
		</tr>
		<tr>
			<th>終了日時</th>
			<td>{$data.db.ad_date_end}</td>
		</tr>
		<tr>
			<th>表示制御</th>
			<td>{if $data.db.ad_is_show == "1"}表示{else}非表示{/if}</td>
		</tr>
		<tr><th colspan="4" class="text-center">広告コード</th></tr>
		<tr>
			<th>PC/タブレット&nbsp;<i class="fa fa-lg fa-desktop"></i></th>
			<td>
				<pre>{$data.db.ad_tag|escape}</pre>
				<hr>
				{$data.db.ad_tag}
			</td>
		</tr>
		<tr>
			<th>モバイル&nbsp;<i class="fa fa-2x fa-mobile"></i></th>
			<td>
				<pre>{$data.db.ad_tag_mobile|escape}</pre>
				<hr>
				{$data.db.ad_tag_mobile}
			</td>
		</tr>
		<tr>
			<th>Androidアプリ&nbsp;<i class="fa fa-lg fa-android"></i></th>
			<td>
				<pre>{$data.db.ad_tag_Android|escape}</pre>
				<hr>
				{$data.db.ad_tag_Android}
			</td>
		</tr>
		<tr>
			<th>iOSアプリ&nbsp;<i class="fa fa-lg fa-apple"></i></th>
			<td>
				<pre>{$data.db.ad_tag_iOS|escape}</pre>
				<hr>
				{$data.db.ad_tag_iOS}
			</td>
		</tr>
		<tr>
			<th>コメント</th>
			<td>{$data.db.comment|nl2br}</td>
		</tr>
		{if !$data.check}
		<tr>
			<th>作成日時</th>
			<td>{$data.db.date_create}</td>
		</tr>
		<tr>
			<th>更新日時</th>
			<td>{$data.db.date_update}</td>
		</tr>
		{/if}
	</tbody>
</table>

{if $data.check}
{* 登録確認の場合は *}
<div class="alert alert-info text-center">
	<p>
	上記の情報を保存しますか？
	</p>
	<br>
	{makelink mode="ad" action="update" value="保存する" class="btn btn-primary"}
</div>
<hr>
<div>
	{makelink mode="ad" action="input" value="編集" class="btn btn-warning"}
</div>
{/if}

{else}
<strong>データは存在しません</strong>
{/if}
{/block}
