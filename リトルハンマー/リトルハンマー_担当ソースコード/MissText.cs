using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using DG.Tweening;
using UniRx;
using UnityEngine.SceneManagement;
public class MissText : MonoBehaviour
{
    static public MissText instance;
    RectTransform rectTransform;
    [SerializeField]GameOver gameOver;
    // Start is called before the first frame update
    void Start()
    {
        if(instance == null)
        {
            instance = this;
        }
        else
        {
            Destroy(this.gameObject);
        }

        rectTransform = GetComponent<RectTransform>();
        rectTransform.anchoredPosition = new Vector3(0, 600, 0);
        gameOver.gameover.Where(x => x == true).Subscribe(_ => ShowText());
        gameObject.SetActive(false);
    }

    void ShowText()
    {
        gameObject.SetActive(true);
        rectTransform.DOLocalMoveY(0f, 2.0f).SetEase(Ease.OutBounce).SetDelay(1.25f).OnComplete(() => Transition.instance.DoTransition(SceneManager.GetActiveScene().name));
    }

    public void Reset()
    {
        rectTransform.anchoredPosition = new Vector3(0, 600, 0);
        gameObject.SetActive(false);
    }
}
