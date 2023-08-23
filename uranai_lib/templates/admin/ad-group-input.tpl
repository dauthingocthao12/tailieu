{extends file="main.tpl"}
{* block name=title}{$data.title}{/block *}
{block name=body}
<style>
#star1_url,
#star2_url,
#star3_url,
#star4_url,
#star5_url,
#star6_url,
#star7_url,
#star8_url,
#star9_url,
#star10_url,
#star11_url,
#star12_url
{
	display: block;
	visibility: visible;
}

.thsub{
	font-size: 12px;
	margin-top: 5px;
}
</style>


{if $data.check.error}
<div class="alert alert-danger">
	{$data.check.error}
</div>
{/if}

<div class="text-right">
	{makelink mode="ad" action="group-listing" value="広告グループ一覧へ戻る" class="btn btn-warning"}
</div>
<br>

<form name="f" action="" method="POST">
	<table class="table table-bordered">
		<tbody>
			<tr>
				<th  width="20%">ID</th>
				<td colspan="2">
					{$data.db.ad_group_id}
				</td>
			</tr>
			<tr>
				<th>広告グループ名</th>
				<td colspan="2">
					<input type="text" name="ad_group_name" value="{$data.db.ad_group_name}" class="form-control">
					{if $data.check.field_errors.ad_group_name}
					<div class="alert alert-danger">{$data.check.field_errors.ad_group_name}</div>
					{/if}
				</td>
			</tr>
			<tr>
				<th>グループ内所属広告</th>
				<td colspan="2">
					<table>
					{foreach from=$data.ad_name_list key=ad_id item=ad_name}
						{assign var=item_checked value=''}
						<tr>
						<td>
							<span style="font-size:12px;">{$ad_id}</span>
						</td>
						</td>
						<td>
						{foreach from=$data.db.ad_ids item=v}
							{if $ad_id == $v}
							{assign var=item_checked value='checked'}
							{/if}
						{/foreach}
							<input type="checkbox" name="ad_ids[]" id="ad_{$ad_id}" value="{$ad_id}" {$item_checked}>
							<label style="font-size:12px; font-weight:normal;" for="ad_{$ad_id}">{$ad_name}</label>
						</td>
						</tr>
					{/foreach}
					</table>
					{if $data.check.field_errors.ad_ids}
					<div class="alert alert-danger">{$data.check.field_errors.ad_ids}</div>
					{/if}
				</td>
			</tr>
		</tbody>
	</table>

	<div class="text-center">
		<input type="hidden" name="mode" value="ad">
		<input type="hidden" name="action" value="group-check"> <!-- insert -->
		<input type="hidden" name="ad_group_id" value="{$data.db.ad_group_id}">
		<input type="submit" value="登録" class="btn btn-primary">
	</div>
</form>

{/block}
