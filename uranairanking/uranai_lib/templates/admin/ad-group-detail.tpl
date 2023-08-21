{extends file="main.tpl"}
{* block name=title}{$data.title}{/block *}
{block name=body}
<!--center><a href="../libadmin/edit.php?id={$datum.site_id}">[編集]</a></center-->

{if $data.db}

{if !$data.check}
{* 登録確認以外の場合は *}
<div class="text-right">
	{makelink mode="ad" action="group-listing" value="一覧へ戻る" class="btn btn-warning"}
	{makelink mode="ad" action="group-input" id="{$data.db.ad_id}" value="編集" class="btn btn-primary"}
</div>
{/if}

<br>

<table class="table table-bordered">
	<tbody>
		<tr>
			<th width="20%">ID</th>
			<td>{$data.db.ad_group_id}</td>
		</tr>
		<tr>
			<th>広告グループ名</th>
			<td>{$data.db.ad_group_name}</td>
		</tr>
		<tr>
			<th>グループ内所属広告</th>
			<td colspan="2">
				<ul>
				{foreach from=$data.ad_name_list key=ad_id item=ad_name}
					{foreach from=$data.db.ad_ids item=v}
						{if $ad_id == $v}
						<li>{$ad_name}</li>
						{/if}
					{/foreach}
				{/foreach}
				</ul>
			</td>
		</tr>
	</tbody>
</table>

{if $data.check}
{* 登録確認の場合は *}
<div class="alert alert-info text-center">
	<p>
	上記の情報を保存しますか？
	</p>
	<br>
	{makelink mode="ad" action="group-update" value="保存する" class="btn btn-primary"}
</div>
<hr>
<div>
	{makelink mode="ad" action="group-input" value="編集" class="btn btn-warning"}
</div>
{/if}

{else}
<strong>データは存在しません</strong>
{/if}
{/block}
