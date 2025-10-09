// 在Part新建几何集
//首先拿到当前编辑器下的根节点，基于根节点创建几何集
/**
 * @brief 创建新的几何集
 * @param ispiProd 根节点
 * @param istrName 当前几何集名称
 * @param ospiSpecGeoSet 当前创建的几何集
 * @return 是否创建成功
 * @owner pxq
 * @note
 */
HRESULT CreateNewGeoSet(CATIProduct_var ispiProd,CATUnicodeString istrName,CATISpecObject_var &ospiSpecGeoSet)
{
	HRESULT rc = S_OK;
	if (ispiProd == NULL_var)
	{
		return E_FAIL;
	}
	CATIProduct_var ispiProdRef = ispiProd->GetReferenceProduct();
	if (ispiProdRef==NULL_var)
	{
		return E_FAIL;
	}
	CATILinkableObject *piLinkableObjOnChild = NULL;
	rc = ispiProdRef->QueryInterface(IID_CATILinkableObject,(void**)&piLinkableObjOnChild);
	if (FAILED(rc))
	{
		return E_FAIL;
	}
	CATDocument *pDocOnChild = NULL;
	pDocOnChild = piLinkableObjOnChild->GetDocument();
	if (NULL == pDocOnChild)
	{
		return E_FAIL;
	}
	CATIContainerOfDocument_var spContOfDocOnChild = pDocOnChild;
	CATIContainer *piSpecContainerOnChild = NULL;
	rc = spContOfDocOnChild->GetSpecContainer(piSpecContainerOnChild);
	if (FAILED(rc))
	{
		return E_FAIL;
	}
	CATIPrtContainer *piPrtContainerOnChild = NULL;
	rc = piSpecContainerOnChild->QueryInterface(IID_CATIPrtContainer,(void**)&piPrtContainerOnChild);
	piSpecContainerOnChild->Release();
	if (FAILED(rc))
	{
		return E_FAIL;
	}
	CATIPrtPart_var spPartOnChild = piPrtContainerOnChild->GetPart();
	piPrtContainerOnChild->Release();
	if (NULL_var == spPartOnChild)
	{
		return E_FAIL;
	}
	CATIPartRequest *pPartAsRequest = NULL;
	rc = spPartOnChild->QueryInterface(IID_CATIPartRequest, (void**)&pPartAsRequest);
	if (FAILED(rc))
	{
		return E_FAIL;
	}
	CATLISTV(CATBaseUnknown_var) lstGeomSet = NULL;
	pPartAsRequest->GetSurfBodies("", lstGeomSet);
	CATISpecObject_var spChildTool = NULL_var;
	if (lstGeomSet == NULL || istrName == "")
	{
		CATISpecObject_var spParentTool = spPartOnChild;
		CATIMechanicalRootFactory_var spMechRootFactory = piSpecContainerOnChild;
		rc = spMechRootFactory->CreateGeometricalSet(istrName,spParentTool,spChildTool,-1);
		if (FAILED(rc))
		{
			return E_FAIL;
		}
		spPartOnChild->SetCurrentFeature(spChildTool);
	}
	ospiSpecGeoSet = spChildTool;
	return rc;
}

/**
 * @brief 将创建的特征插入几何集
 * @param pTempSpec 特征对象
 * @param ispiSpecGeoSet 当前几何集名称
 * @return 是否创建成功
 * @owner pxq
 * @note
 */
HRESULT InsertFeature(CATISpecObject * pTempSpec,CATISpecObject_var ispiSpecGeoSet)
{
	if(pTempSpec ==NULL)
		return E_FAIL;
	CATIGSMProceduralView_var spiProceduralView = pTempSpec;
	if (NULL_var != spiProceduralView )
	{
		rc = spiProceduralView->InsertInProceduralView(ispiSpecGeoSet);
	}
	if (ispiSpecGeoSet != NULL_var)
	{
		ispiSpecGeoSet->Update();
	}
	return S_OK;
}
