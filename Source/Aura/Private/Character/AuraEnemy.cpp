// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AuraEnemy.h"

#include "Aura/Aura.h"

/**
 * AAuraEnemy 构造函数
 * 初始化敌人角色的碰撞设置
 */
AAuraEnemy::AAuraEnemy()
{
	// 设置网格体对可见性通道的碰撞响应为忽略
	// 这样射线检测（如鼠标悬停检测）不会被网格体阻挡
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
}

/**
 * 高亮显示Actor
 * 当鼠标悬停在敌人上时调用，启用自定义深度渲染以实现轮廓高亮效果
 */
void AAuraEnemy::HighlightActor()
{
	// 启用网格体的自定义深度渲染
	GetMesh()->SetRenderCustomDepth(true);
	// 设置自定义深度模板值为红色（用于后处理材质识别）
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	// 启用武器的自定义深度渲染
	Weapon ->SetRenderCustomDepth(true);
	// 设置武器的自定义深度模板值为红色
	Weapon ->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
}

/**
 * 取消高亮显示Actor
 * 当鼠标离开敌人时调用，关闭自定义深度渲染以取消轮廓高亮效果
 */
void AAuraEnemy::unHighlightActor()
{
	// 关闭网格体的自定义深度渲染
	GetMesh()->SetRenderCustomDepth(false);
	// 关闭武器的自定义深度渲染
	Weapon ->SetRenderCustomDepth(false);
}
