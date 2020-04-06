Page({

  /**
   * 页面的初始数据
   */
  data: {
    messageList: [],
    inputtext: '',
    scrollTop: 0,
  },
  mydata: {
    message: '',
    btnname: "开启小夜灯",
  },
  mDNSstate: false,

  /**
   * 生命周期函数--监听页面加载
   */
  onLoad: function (options) {
    var that = this
    that.setData({
      btnname: "一键三连",
    })
    //开始搜索局域网下的 mDNS 服务。搜索的结果会通过 wx.onLocalService事件返回
    wx.startLocalServiceDiscovery({
      serviceType: '_websocket._tcp.', //搜索 _websocket._tcp. 类型的服务
      success: function (e) {
        //console.log(e)
        var data = that.showMessage("left", "开始mDNS搜索 (仅支持一个设备)"); //定义函数的格式，func为函数名字 

        wx.showLoading({ //开启搜索
          title: '设备搜索中30s',
          mask: true
        })   

      },
      fail: console.log
    })

    // 监听服务发现事件
    wx.onLocalServiceFound(function (obj) {
      //console.log(obj)
      var data = that.showMessage("left", "设备名：" + obj.serviceName + "  服务类型：" + obj.serviceType); //定义函数的格式，func为函数名字
      var data = that.showMessage("left", "IP端口：" + obj.ip + ":" + obj.port);
      wx.stopLocalServiceDiscovery({
        success: function (e) {
          that.mDNSstate = 'true'
          //console.log(that.mDNSstate)
        },
      })
      wx.connectSocket({
        url: 'ws://' + obj.ip + ':' + obj.port,
        success: function (e) {
          //console.log(e)
        },
        fail: function (e) {
          console.log(e)
        },
        complete: function (e) {
          //console.log(e)         
        }
      })

      wx.onSocketMessage(function (res) {
        var data = that.showMessage("left", "服务器：" + res.data)
      })
    })

    // 监听mDNS服务解析失败事件
    wx.onLocalServiceResolveFail(function (obj) {
      var data = that.showMessage(0, "mDNS服务解析失败");
    })

    // 监听mDNS服务离开
    wx.onLocalServiceLost(function (obj) {
      var data = that.showMessage(0, "mDNS服务断开连接");
    })

    // 监听mDNS停止搜索搜索
    wx.onLocalServiceDiscoveryStop(function (obj) {
      //console.log(that.mDNSstate)
      wx.hideLoading() //隐藏提示框
      if (that.mDNSstate) {
        var data = that.showMessage(0, "开始连接");
      } else {
        var data = that.showMessage(0, "停止搜索 未发现可用设备");
      }
    })

  },


  //获取文本框的点击事件
  // butText: function () {
  //   console.log("button1")
  //   var that = this
  // },

  //获取输入文本框内容
  input1(event) {
    //console.log(event)
    this.mydata.message = event.detail.value
  },


  //发送按钮监听事件
  button1: function () {
    var that = this
    // if (this.mydata.isSend) {
    //   return;
    // }
    // this.mydata.isSend = true
    let message = this.mydata.message
    if (message.trim() === '') {
      wx.showToast({
        title: '请输入内容',
      })
      return;
    }
    wx.sendSocketMessage({
      data: message,
      success: function (e) {
        var data = that.showMessage(0, that.mydata.message)
        that.setData({
          inputtext: '',
        })
      },
      // fail: function (e) {
      //   console.log(e)
      // },
      // complete: function (e) {
      //   console.log(e)
      // }
    });
  },


  button2(e) {
    var that = this
    if (e.target.dataset.btnname == "一键三连") {
      that.setData({
        btnname: "关闭三连",
      })
      wx.sendSocketMessage({ //发送命令 
        data: "openlight",
        success: function (e) {
          that.showMessage(0, "openlight")
        }
      })
    } else {
      that.setData({
        btnname: "一键三连",
      })
      wx.sendSocketMessage({ //发送命令 
        data: "closelight",
        success: function (e) {
          that.showMessage(0, "closelight")
        }
      })
    }
  },

  button3(e) {
    var that = this
    wx.sendSocketMessage({ //发送命令 
      data: "nightlight",
      success: function (e) {
        that.showMessage(0, "nightlight")
      }
    })
  },

  button4(e) {
    var that = this
    wx.sendSocketMessage({ //发送命令 
      data: "colorslight",
      success: function (e) {
        that.showMessage(0, "colorslight")
      }
    })
  },

  button5(e) {
    var that = this
    wx.sendSocketMessage({ //发送命令 
      data: "breathinglight",
      success: function (e) {
        that.showMessage(0, "breathinglight")
      }
    })
  },

  button6(e) {
    var that = this
    wx.sendSocketMessage({ //发送命令 
      data: "randomlight",
      success: function (e) {
        that.showMessage(0, "randomlight")
      }
    })
  },

  button7(e) {
    //this.showMessage(0, "ceshi")
    var that = this
    wx.sendSocketMessage({ //发送命令 
      data: "closelight",
      success: function (e) {
        that.showMessage(0, "closelight")
      }
    })
  },

  //自定义带参函数
  showMessage: function (textAlign, text) {
    //console.log("显示请求" + textAlign + text)
    let list = this.data.messageList; //获取屏幕现有数据
    let obj = {
      textAlign: textAlign,
      text: text
    }
    list.push(obj) //push() 方法可向数组的末尾添加一个或多个元素
    this.setData({
      messageList: list
    })
    this.setData({  //始终滚动到最新发送的信息
      scrollTop: 1000 // 这里我们的单对话区域最高1000，取了最大值，应该有方法取到精确的
    });
  },

  /**
   * 生命周期函数--监听页面初次渲染完成
   */
  onReady: function () {

  },

  /**
   * 生命周期函数--监听页面显示
   */
  onShow: function () {

  },

  /**
   * 生命周期函数--监听页面隐藏
   */
  onHide: function () {

  },

  /**
   * 生命周期函数--监听页面卸载
   */
  onUnload: function () {

  },

  /**
   * 页面相关事件处理函数--监听用户下拉动作
   */
  onPullDownRefresh: function () {

  },

  /**
   * 页面上拉触底事件的处理函数
   */
  onReachBottom: function () {

  },

  /**
   * 用户点击右上角分享
   */
  onShareAppMessage: function () {

  }
})